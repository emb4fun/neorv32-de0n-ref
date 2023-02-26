// #################################################################################################
// # << NEORV32: legacy.h - Backwards compatibility wrappers and functions >>                      #
// # ********************************************************************************************* #
// # BSD 3-Clause License                                                                          #
// #                                                                                               #
// # Copyright (c) 2023, Stephan Nolting. All rights reserved.                                     #
// #                                                                                               #
// # Redistribution and use in source and binary forms, with or without modification, are          #
// # permitted provided that the following conditions are met:                                     #
// #                                                                                               #
// # 1. Redistributions of source code must retain the above copyright notice, this list of        #
// #    conditions and the following disclaimer.                                                   #
// #                                                                                               #
// # 2. Redistributions in binary form must reproduce the above copyright notice, this list of     #
// #    conditions and the following disclaimer in the documentation and/or other materials        #
// #    provided with the distribution.                                                            #
// #                                                                                               #
// # 3. Neither the name of the copyright holder nor the names of its contributors may be used to  #
// #    endorse or promote products derived from this software without specific prior written      #
// #    permission.                                                                                #
// #                                                                                               #
// # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS   #
// # OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               #
// # MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    #
// # COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     #
// # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE #
// # GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED    #
// # AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     #
// # NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED  #
// # OF THE POSSIBILITY OF SUCH DAMAGE.                                                            #
// # ********************************************************************************************* #
// # The NEORV32 Processor - https://github.com/stnolting/neorv32              (c) Stephan Nolting #
// #################################################################################################


/**********************************************************************//**
 * @file legacy.h
 * @brief Wrappers and functions for backwards compatibility.
 * @warning Do not use these functions for new designs as they are not supported and might get removed in the future.
 **************************************************************************/

#ifndef neorv32_legacy_h
#define neorv32_legacy_h


// ================================================================================================
// UART0 & UART1
// ================================================================================================

/**********************************************************************//**
 * Print string (zero-terminated) via UART0. Print full line break "\r\n" for every '\n'.
 * @note This function is blocking.
 * @warning This function is deprecated!
 * @param[in] s Pointer to string.
 **************************************************************************/
inline void __attribute__((deprecated("Use 'neorv32_uart0_puts()' instead."))) neorv32_uart0_print(const char *s) {
  neorv32_uart0_puts(s);
}

/**********************************************************************//**
 * Print string (zero-terminated) via UART1. Print full line break "\r\n" for every '\n'.
 * @note This function is blocking.
 * @warning This function is deprecated!
 * @param[in] s Pointer to string.
 **************************************************************************/
inline void __attribute__((deprecated("Use 'neorv32_uart0_puts()' instead."))) neorv32_uart1_print(const char *s) {
  neorv32_uart1_puts(s);
}


// ================================================================================================
// NEORV32 Runtime Environment (RTE)
// ================================================================================================

/**********************************************************************//**
 * Install trap handler function to NEORV32 runtime environment.
 * @warning This function is deprecated!
 * @param[in] id Identifier (type) of the targeted trap. See #NEORV32_RTE_TRAP_enum.
 * @param[in] handler The actual handler function for the specified trap (function MUST be of type "void function(void);").
 * @return 0 if success, 1 if error (invalid id or targeted trap not supported).
 **************************************************************************/
inline int __attribute__((deprecated("Use 'neorv32_rte_handler_install()' instead."))) neorv32_rte_exception_install(uint8_t id, void (*handler)(void)) {
  return neorv32_rte_handler_install(id, handler);
}

/**********************************************************************//**
 * Uninstall trap handler function from NEORV32 runtime environment, which was
 * previously installed via neorv32_rte_exception_install(uint8_t id, void (*handler)(void)).
 * @warning This function is deprecated!
 * @param[in] id Identifier (type) of the targeted trap. See #NEORV32_RTE_TRAP_enum.
 * @return 0 if success, 1 if error (invalid id or targeted trap not supported).
 **************************************************************************/
inline int __attribute__((deprecated("Use 'neorv32_rte_handler_uninstall()' instead."))) neorv32_rte_exception_uninstall(uint8_t id) {
  return neorv32_rte_handler_uninstall(id);
}


// ================================================================================================
// Custom Functions Unit (CFU)
// ================================================================================================

/**********************************************************************//**
 * @name Backward-compatibility layer (before version v1.7.8.2)
 * @warning This function is deprecated!
 **************************************************************************/
/**@{*/
/** R3-type CFU custom instruction 0 (funct3 = 000) */
#define neorv32_cfu_cmd0(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 0, rs1, rs2)
/** R3-type CFU custom instruction 1 (funct3 = 001) */
#define neorv32_cfu_cmd1(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 1, rs1, rs2)
/** R3-type CFU custom instruction 2 (funct3 = 010) */
#define neorv32_cfu_cmd2(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 2, rs1, rs2)
/** R3-type CFU custom instruction 3 (funct3 = 011) */
#define neorv32_cfu_cmd3(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 3, rs1, rs2)
/** R3-type CFU custom instruction 4 (funct3 = 100) */
#define neorv32_cfu_cmd4(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 4, rs1, rs2)
/** R3-type CFU custom instruction 5 (funct3 = 101) */
#define neorv32_cfu_cmd5(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 5, rs1, rs2)
/** R3-type CFU custom instruction 6 (funct3 = 110) */
#define neorv32_cfu_cmd6(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 6, rs1, rs2)
/** R3-type CFU custom instruction 7 (funct3 = 111) */
#define neorv32_cfu_cmd7(funct7, rs1, rs2) neorv32_cfu_r3_instr(funct7, 7, rs1, rs2)
/**@}*/


// ================================================================================================
// CPU Core
// ================================================================================================

/**********************************************************************//**
 * Get current system time from time[h] CSR.
 * @note This function requires the MTIME system timer to be implemented.
 * @return Current system time (64 bit).
 **************************************************************************/
inline uint64_t __attribute__((deprecated("Use 'neorv32_mtime_get_time()' instead."))) neorv32_cpu_get_systime(void) {
  return neorv32_mtime_get_time();
}

/**********************************************************************//**
 * Enable global CPU interrupts (via MIE flag in mstatus CSR).
 * @note Interrupts are always enabled when the CPU is in user-mode.
 **************************************************************************/
inline void __attribute__ ((always_inline, deprecated("Use 'neorv32_cpu_csr_set(CSR_MSTATUS, 1 << CSR_MSTATUS_MIE)' instead."))) neorv32_cpu_eint(void) {

  neorv32_cpu_csr_set(CSR_MSTATUS, 1 << CSR_MSTATUS_MIE);
}

/**********************************************************************//**
 * Disable global CPU interrupts (via MIE flag in mstatus CSR).
 * @note Interrupts are always enabled when the CPU is in user-mode.
 **************************************************************************/
inline void __attribute__ ((always_inline, deprecated("Use 'neorv32_cpu_csr_clr(CSR_MSTATUS, 1 << CSR_MSTATUS_MIE)' instead."))) neorv32_cpu_dint(void) {

  neorv32_cpu_csr_clr(CSR_MSTATUS, 1 << CSR_MSTATUS_MIE);
}

/**********************************************************************//**
 * Enable specific interrupt source.
 * @param[in] irq_sel CPU interrupt select. See #NEORV32_CSR_MIE_enum.
 * @return 0 if success, 1 if error (invalid irq_sel).
 **************************************************************************/
inline int __attribute__((deprecated("Use 'neorv32_cpu_csr_set(CSR_MSTATUS, 1 << channel)' instead."))) neorv32_cpu_irq_enable(uint8_t irq_sel) {

  neorv32_cpu_csr_set(CSR_MSTATUS, 1 << irq_sel);
  return 0;
}

/**********************************************************************//**
 * Disable specific interrupt source.
 * @param[in] irq_sel CPU interrupt select. See #NEORV32_CSR_MIE_enum.
 * @return 0 if success, 1 if error (invalid irq_sel).
 **************************************************************************/
inline int __attribute__((deprecated("Use 'neorv32_cpu_csr_clr(CSR_MSTATUS, 1 << channel)' instead."))) neorv32_cpu_irq_disable(uint8_t irq_sel) {

  neorv32_cpu_csr_clr(CSR_MSTATUS, 1 << irq_sel);
  return 0;
}

#endif // neorv32_legacy_h
