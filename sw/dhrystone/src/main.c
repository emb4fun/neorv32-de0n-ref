/**************************************************************************
*  Copyright (c) 2023 by Michael Fischer (www.emb4fun.de).
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
**************************************************************************/
#define __MAIN_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdint.h>
#include "neorv32.h"
#include "dhrystone.h"
#include "dhry_port.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  HardwareInit                                                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void HardwareInit (void)
{
   /* 
    * This is from the NEORV32 Dhrystone example 
    */
   
   neorv32_cpu_csr_write(CSR_MIE, 0); // no interrupt, thanks
   neorv32_rte_setup(); // capture all exceptions and give debug information, ho hw flow control
   neorv32_uart0_setup(19200, 0);
   // check available hardware extensions and compare with compiler flags
   neorv32_rte_check_isa(0); // silent = 0 -> show message if isa mismatch
   
} /* HardwareInit */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  main                                                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
int main (void)
{
   /************************* "Debugger test" *************************/

   uint32_t dStart = 0;
   uint32_t dEnd   = 0;
   uint32_t dSize  = 0;

   dStart = 0x90000000;
   dEnd   = 0x92000000;
   dSize  = (dEnd - dStart) - 1;

   __asm__ volatile ("nop");
   
   /*******************************************************************/

   /*
    * Before the application which is using the NEORV32 can be started, 
    * the hardware must be initialized.
    */
   HardwareInit();

   _printf("Processor running at %u Hz\n", (uint32_t)NEORV32_SYSINFO->CLK);
   _printf("Executing Dhrystone (%u iterations). This may take some time...\n", (uint32_t)NUMBER_OF_RUNS);

   /*
    * Start Dhrystone benchmark
    */
   main_dhry();
   

   /*
    * Stop here
    */
   while (1)
   {
      __asm__ volatile ("nop");
   }
        
   /*
    * This return here make no sense.
    * But to prevent the compiler warning:
    *    "return type of 'main' is not 'int'
    * We use an int as return :-)
    */ 
   return(dSize); /*lint !e527*/
} /* main */

/*** EOF ***/
