-- #################################################################################################
-- # << NEORV32 - Generic Single-Clock FIFO >>                                                     #
-- # ********************************************************************************************* #
-- # BSD 3-Clause License                                                                          #
-- #                                                                                               #
-- # Copyright (c) 2023, Stephan Nolting. All rights reserved.                                     #
-- #                                                                                               #
-- # Redistribution and use in source and binary forms, with or without modification, are          #
-- # permitted provided that the following conditions are met:                                     #
-- #                                                                                               #
-- # 1. Redistributions of source code must retain the above copyright notice, this list of        #
-- #    conditions and the following disclaimer.                                                   #
-- #                                                                                               #
-- # 2. Redistributions in binary form must reproduce the above copyright notice, this list of     #
-- #    conditions and the following disclaimer in the documentation and/or other materials        #
-- #    provided with the distribution.                                                            #
-- #                                                                                               #
-- # 3. Neither the name of the copyright holder nor the names of its contributors may be used to  #
-- #    endorse or promote products derived from this software without specific prior written      #
-- #    permission.                                                                                #
-- #                                                                                               #
-- # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS   #
-- # OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               #
-- # MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    #
-- # COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     #
-- # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE #
-- # GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED    #
-- # AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     #
-- # NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED  #
-- # OF THE POSSIBILITY OF SUCH DAMAGE.                                                            #
-- # ********************************************************************************************* #
-- # The NEORV32 Processor - https://github.com/stnolting/neorv32              (c) Stephan Nolting #
-- #################################################################################################

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library neorv32;
use neorv32.neorv32_package.all;

entity neorv32_fifo is
  generic (
    FIFO_DEPTH : natural; -- number of fifo entries; has to be a power of two; min 1
    FIFO_WIDTH : natural; -- size of data elements in fifo
    FIFO_RSYNC : boolean; -- false = async read; true = sync read
    FIFO_SAFE  : boolean; -- true = allow read/write only if entry available
    FIFO_GATE  : boolean  -- true = use output gate (set to zero if no valid data available)
  );
  port (
    -- control --
    clk_i   : in  std_ulogic; -- clock, rising edge
    rstn_i  : in  std_ulogic; -- async reset, low-active
    clear_i : in  std_ulogic; -- sync reset, high-active
    half_o  : out std_ulogic; -- FIFO is at least half full
    -- write port --
    wdata_i : in  std_ulogic_vector(FIFO_WIDTH-1 downto 0); -- write data
    we_i    : in  std_ulogic; -- write enable
    free_o  : out std_ulogic; -- at least one entry is free when set
    -- read port --
    re_i    : in  std_ulogic; -- read enable
    rdata_o : out std_ulogic_vector(FIFO_WIDTH-1 downto 0); -- read data
    avail_o : out std_ulogic  -- data available when set
  );
end neorv32_fifo;

architecture neorv32_fifo_rtl of neorv32_fifo is

  -- FIFO --
  type fifo_data_t is array (0 to FIFO_DEPTH-1) of std_ulogic_vector(FIFO_WIDTH-1 downto 0);
  type fifo_t is record
    we    : std_ulogic; -- write enable
    re    : std_ulogic; -- read enable
    w_pnt : std_ulogic_vector(index_size_f(FIFO_DEPTH) downto 0); -- write pointer
    r_pnt : std_ulogic_vector(index_size_f(FIFO_DEPTH) downto 0); -- read pointer
    data  : fifo_data_t; -- fifo memory
    buf   : std_ulogic_vector(FIFO_WIDTH-1 downto 0); -- if single-entry FIFO
    match : std_ulogic;
    empty : std_ulogic;
    full  : std_ulogic;
    free  : std_ulogic;
    avail : std_ulogic;
  end record;
  signal fifo : fifo_t;

  -- misc --
  signal rdata      : std_ulogic_vector(FIFO_WIDTH-1 downto 0);
  signal level_diff : std_ulogic_vector(index_size_f(FIFO_DEPTH) downto 0);

begin

  -- Sanity Checks --------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  assert not (FIFO_DEPTH = 0) report "NEORV32 CONFIG ERROR: FIFO depth has to be > 0." severity error;
  assert not (is_power_of_two_f(FIFO_DEPTH) = false) report "NEORV32 CONFIG ERROR: FIFO depth has to be a power of two." severity error;


  -- Access Control -------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  fifo.re <= re_i when (FIFO_SAFE = false) else (re_i and fifo.avail); -- SAFE = read only if data available
  fifo.we <= we_i when (FIFO_SAFE = false) else (we_i and fifo.free); -- SAFE = write only if space left


  -- FIFO Pointers --------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  fifo_pointers: process(rstn_i, clk_i)
  begin
    if (rstn_i = '0') then
      fifo.w_pnt <= (others => '0');
      fifo.r_pnt <= (others => '0');
    elsif rising_edge(clk_i) then
      -- write port --
      if (clear_i = '1') then
        fifo.w_pnt <= (others => '0');
      elsif (fifo.we = '1') then
        fifo.w_pnt <= std_ulogic_vector(unsigned(fifo.w_pnt) + 1);
      end if;
      -- read port --
      if (clear_i = '1') then
        fifo.r_pnt <= (others => '0');
      elsif (fifo.re = '1') then
        fifo.r_pnt <= std_ulogic_vector(unsigned(fifo.r_pnt) + 1);
      end if;
    end if;
  end process fifo_pointers;


  -- FIFO Status ----------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  check_large:
  if (FIFO_DEPTH > 1) generate
    fifo.match <= '1' when (fifo.r_pnt(fifo.r_pnt'left-1 downto 0) = fifo.w_pnt(fifo.w_pnt'left-1 downto 0)) else '0';
    fifo.full  <= '1' when (fifo.r_pnt(fifo.r_pnt'left) /= fifo.w_pnt(fifo.w_pnt'left)) and (fifo.match = '1') else '0';
    fifo.empty <= '1' when (fifo.r_pnt(fifo.r_pnt'left)  = fifo.w_pnt(fifo.w_pnt'left)) and (fifo.match = '1') else '0';
  end generate; -- /check_large
  check_small:
  if (FIFO_DEPTH <= 1) generate
    fifo.match <= '1' when (fifo.r_pnt(0) = fifo.w_pnt(0)) else '0';
    fifo.full  <= not fifo.match;
    fifo.empty <= fifo.match;
  end generate; -- /check_small

  fifo.free  <= not fifo.full;
  fifo.avail <= not fifo.empty;

  free_o  <= fifo.free;
  avail_o <= fifo.avail;

  fifo_half_level_simple:
  if (FIFO_DEPTH = 1) generate
    half_o <= fifo.full;
  end generate; -- /fifo_half_level_simple

  fifo_half_level_complex:
  if (FIFO_DEPTH > 1) generate
    level_diff <= std_ulogic_vector(unsigned(fifo.w_pnt) - unsigned(fifo.r_pnt));
    half_o     <= level_diff(level_diff'left-1) or fifo.full;
  end generate; -- /fifo_half_level_complex


  -- FIFO Memory - Write --------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- "real" FIFO memory (several entries) --
  fifo_memory:
  if (FIFO_DEPTH > 1) generate
    fifo_write: process(clk_i)
    begin
      if rising_edge(clk_i) then
        if (fifo.we = '1') then
          fifo.data(to_integer(unsigned(fifo.w_pnt(fifo.w_pnt'left-1 downto 0)))) <= wdata_i;
        end if;
      end if;
    end process fifo_write;
    fifo.buf <= (others => '0'); -- unused
  end generate; -- /fifo_memory

  -- simple register/buffer (single entry) --
  fifo_buffer:
  if (FIFO_DEPTH = 1) generate
    fifo_write: process(clk_i)
    begin
      if rising_edge(clk_i) then
        if (fifo.we = '1') then
          fifo.buf <= wdata_i;
        end if;
      end if;
    end process fifo_write;
    fifo.data <= (others => (others => '0')); -- unused
  end generate; -- /fifo_buffer


  -- FIFO Memory - Read ---------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- "asynchronous" read --
  fifo_read_async:
  if (FIFO_RSYNC = false) generate
    fifo_read: process(fifo)
    begin
      if (FIFO_DEPTH = 1) then
        rdata <= fifo.buf;
      else
        rdata <= fifo.data(to_integer(unsigned(fifo.r_pnt(fifo.r_pnt'left-1 downto 0))));
      end if;
    end process fifo_read;
  end generate; -- /fifo_read_async

  -- synchronous read --
  fifo_read_sync:
  if (FIFO_RSYNC = true) generate
    fifo_read: process(clk_i)
    begin
      if rising_edge(clk_i) then
        if (FIFO_DEPTH = 1) then
          rdata <= fifo.buf;
        else
          rdata <= fifo.data(to_integer(unsigned(fifo.r_pnt(fifo.r_pnt'left-1 downto 0))));
        end if;
      end if;
    end process fifo_read;
  end generate; -- /fifo_read_sync


  -- Output Gate ----------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- Since the FIFO memory (block RAM) does not have a reset, this option can be used to
  -- ensure the output data is always *defined* (by setting the output to all-zero if
  -- not valid data is available).
  rdata_o <= rdata when ((FIFO_GATE = false) or (fifo.avail = '1')) else (others => '0');


end neorv32_fifo_rtl;
