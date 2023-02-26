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
#include <stdio.h>
#include "tal.h"
#include "terminal.h"
#include "xmempool.h"
#include "mbedtls/version.h"

void hash_Test (void);
void ecdsa_Test (void);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (StartStack,  TASK_START_STK_SIZE);
static OS_STACK (LEDStack,    TASK_LED_STK_SIZE);

static OS_TCB TCBStartTask;
static OS_TCB TCBLed;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  EntropyInit                                                          */
/*                                                                       */
/*  Initialize the entropy source.                                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void EntropyInit (void)
{
   uint64_t Seed1, Seed2;
   
   Seed1 = 0;
   Seed2 = 0;

   /*
    * Get the seed from the hardware random number generator
    */
   tal_CPURngInit();
   tal_CPURngHardwarePoll((uint8_t*)&Seed1, sizeof(Seed1));
   tal_CPURngHardwarePoll((uint8_t*)&Seed2, sizeof(Seed2));
   
   (void)Seed1;
   (void)Seed2;
} /* EntropyInit */   

/*************************************************************************/
/*  OutputBootMessage                                                    */
/*                                                                       */
/*  Output boot message.                                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputBootMessage (void)
{
   const char ResetScreen[] = { 0x1B, 'c', 0 };
   
   term_printf("%s", ResetScreen);
   OS_TimeDly(50);   
   
   term_printf("\r\n");
   term_printf("*********************************\r\n");
   term_printf("  Project: %s\r\n", PROJECT_NAME);
   term_printf("  Board  : %s\r\n", TAL_BOARD);
   term_printf("  Version: v%s\r\n", PROJECT_VER_STRING);
   term_printf("  Build  : "__DATE__ " " __TIME__"\r\n");
   term_printf("*********************************\r\n");
   term_printf("\r\n");
   
} /* OutputBootMessage */

/*************************************************************************/
/*  OutputVersionInfo                                                    */
/*                                                                       */
/*  Output version information.                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputVersionInfo (void)
{
   term_printf("*** Version ***\r\n");
   term_printf("Board: %s\r\n", TAL_BOARD);
   term_printf("CPU  : %s\r\n", TAL_CPU);
   term_printf("OS   : %s v%s\r\n", OS_NAME, OS_VER_STRING);
   term_printf("TAL  : v%s\r\n", TAL_CORE_VER_STRING);
   term_printf("TLS  : %s\r\n", MBEDTLS_VERSION_STRING_FULL);
   term_printf("\r\n");
   
} /* OutputVersionInfo */

/*************************************************************************/
/*  OutputFrequencyInfo                                                  */
/*                                                                       */
/*  Output frequency information.                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputFrequencyInfo (void)
{
   term_printf("*** Frequency info ***\r\n");
   term_printf("CPU: %3d MHz\r\n", tal_CPUGetFrequencyCPU() / 1000000); 
   term_printf("\r\n");
   
} /* OutputFrequencyInfo */

/*************************************************************************/
/*  OutputCPULoad                                                        */
/*                                                                       */
/*  Output load information.                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void OutputCPULoad (void)
{
   term_printf("CPU load: %d%%\r\n\r\n", OS_StatGetCPULoad());
   
} /* OutputCPULoad */

/*************************************************************************/
/*  OutputUsageInfo                                                      */
/*                                                                       */
/*  Output usage information.                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputUsageInfo (void)
{
   term_printf("*** Usage ***\r\n");
   term_printf("h: output this info\r\n");
   term_printf("?: output this info\r\n");
   term_printf("c: output cpu load info\r\n");
   term_printf("f: output frequency info\r\n");
   term_printf("r: output runtime stack info\r\n");
   term_printf("t: output task info\r\n");
   term_printf("m: output memory info\r\n");
   term_printf("v: output version info\r\n");
   term_printf("x: reboot\r\n");
   term_printf("\r\n");
   
} /* OutputUsageInfo */

/*************************************************************************/
/*  LEDTask                                                              */
/*                                                                       */
/*  This is the LED task.                                                */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void LEDTask (void *p)
{
   (void)p;
   
   while(1)
   {
      tal_LEDToggle(TAL_LED_CHANNEL_8);
      OS_TimeDly(TASK_LED_DELAY_MS);
   }
   
} /* LEDTask */

/*************************************************************************/
/*  StartTask                                                            */
/*                                                                       */
/*  This is the Start task.                                              */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void StartTask (void *p)
{
   (void)p;

   /*
    * The StartTask will be used to start all other tasks in the system.
    * At the end the priority will be set to "IDLE" priority.
    */
    
   OS_SysTickStart();   /* Start the System ticker */    
   OS_StatEnable();     /* Enable the statistic function  */

   /*******************************************************************/

   term_Start();        /* Start the Terminal functionality */
   
   /*
    * Output startup messages
    */
   OutputBootMessage();
   //OutputFrequencyInfo();
   //OutputUsageInfo();
   
   /* Create the LED task */
   OS_TaskCreate(&TCBLed, LEDTask, NULL, TASK_LED_PRIORITY,
                 LEDStack, sizeof(LEDStack), 
                 "LEDTask");

   EntropyInit();    /* Initialize the entropy source */

   hash_Test();
   ecdsa_Test();

   OS_TaskChangePriority(TASK_START_PRIORITY_IDLE);

   while (1)
   {
      OS_TimeDly(TASK_START_DELAY_MS);
   }
   
} /* StartTask */

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
    * Init the "Tiny Abstraction Layer"
    */
   tal_Init();

   /*
    * Initialize the memory pool
    */
   xmem_Init();

   /* 
    * Create the StartTask.
    * The StartTask is the one and only task which 
    * will start all other tasks of the system.
    */
   OS_TaskCreate(&TCBStartTask, StartTask, NULL, TASK_START_PRIORITY,
                 StartStack, sizeof(StartStack), 
                 "StartTask");
   
   /* 
    * OSStart must be the last function here.
    *  
    * Fasten your seatbelt, engine will be started...
    */        
   OS_Start();
        
   /*
    * This return here make no sense.
    * But to prevent the compiler warning:
    *    "return type of 'main' is not 'int'
    * We use an int as return :-)
    */ 
   return(dSize); /*lint !e527*/
} /* main */

/*************************************************************************/
/*  term_RxCallback                                                      */
/*                                                                       */
/*  Will be called from TermTask in case a char is received.             */
/*                                                                       */
/*  In    : bData                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void term_RxCallback (uint8_t bData)
{
   switch (bData)
   {
      case 'h':
      case '?':
      {
         OutputUsageInfo();
         break;
      } 
           
      case 'c':
      {
         OutputCPULoad();
         break;
      }

      case 'f':
      {
         OutputFrequencyInfo();
         break;
      }

      case 'r':
      {
         OS_OutputRuntimeStackInfo();
         break;
      }
      
      case 't':
      {
         OS_OutputTaskInfo();
         break;
      }
      
      case 'm':
      {
         tal_MEMOutputMemoryInfo();
         break;
      }
      
      case 'v':
      {
         OutputVersionInfo();
         break;
      }
      
      case 'x':
      {
         tal_CPUReboot();
         break;
      }
      
      /****************************************************/
      
      default:
      {
         /* Do nothing */
         break;
      }
   } /* end switch (bData) */
   
} /* term_RxCallback */

/*************************************************************************/
/*  mbedtls_hardware_poll                                                */
/*                                                                       */
/*  mbedTLS support function to retrieve RNG data.                       */
/*                                                                       */
/*  In    : data, output, olen                                           */
/*  Out   : output, olen                                                 */
/*  Return: 0 = OK                                                       */
/*************************************************************************/
int mbedtls_hardware_poll (void *data, unsigned char *output, size_t len, size_t *olen)
{
   (void)data;

   tal_CPURngHardwarePoll(output, len);
   *olen = len;
   
   return(0);
} /* mbedtls_hardware_poll */

/*** EOF ***/
