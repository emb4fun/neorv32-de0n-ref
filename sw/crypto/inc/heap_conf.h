/**************************************************************************
*  Copyright (c) 2020 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
***************************************************************************
*  History:
*
*  06.09.2020  mifi  First Version.
**************************************************************************/
#if !defined(__HEAP_CONF_H__)
#define __HEAP_CONF_H__

/**************************************************************************
*  Includes
**************************************************************************/

/**************************************************************************
*  Global Definitions
**************************************************************************/

#if defined(__FLASH__)
#define TAL_HEAP_MEM1_START   __SDRAM_segment_start__
#define TAL_HEAP_MEM1_END     __SDRAM_segment_end__
#endif

#if defined(__TCM__)
#define TAL_HEAP_MEM1_START   __SDRAM_segment_start__
#define TAL_HEAP_MEM1_END     __SDRAM_segment_end__
#endif

#if defined(__SDRAM__)
#define TAL_HEAP_MEM1_START   __SDRAM_segment_used_end__
#define TAL_HEAP_MEM1_END     __SDRAM_segment_end__
#endif

#if defined(__FLASH_2_SDRAM__)
#define TAL_HEAP_MEM1_START   __SDRAM_segment_used_end__
#define TAL_HEAP_MEM1_END     __SDRAM_segment_end__
#endif


/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/
 
#endif /* !__HEAP_CONF_H__ */

/*** EOF ***/
