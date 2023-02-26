/**************************************************************************
*  Copyright (c) 2019 by Michael Fischer (www.emb4fun.de).
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
*  20.06.2019  mifi  First Version.
**************************************************************************/
#define __HASH_TEST_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include "tal.h"
#include "terminal.h"

#include "mbedtls/version.h"
#include "mbedtls/md5.h"
#include "mbedtls/ripemd160.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define HASH_DATA_CHUNK   65536

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all extern Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*
 * Benchmark test data start
 */
static const uint8_t *pTestData = (uint8_t*)0x90000000;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  MD5Test                                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void MD5Test (void)
{
   uint32_t           dStartTime;
   uint32_t           dEndTime;
   uint32_t           dCount;
   float              fExecTime;
   float              fValue;
   mbedtls_md5_context ctx;
   uint8_t             HashValue[16];
   int                nR1, nR2;
   

   dCount     = 0;
   dStartTime = OS_TimeGet();
   dEndTime   = dStartTime + OS_MS_2_TICKS(1000);

   /* Start the HASH */
   mbedtls_md5_init(&ctx);
   mbedtls_md5_starts_ret(&ctx);

   do
   {
      /* Calculate the hash over the data */
      mbedtls_md5_update_ret(&ctx, (uint8_t*)pTestData, HASH_DATA_CHUNK);
      dCount += HASH_DATA_CHUNK;
   } 
   while (OS_TimeGet() < dEndTime); 

   /* Get result */
   mbedtls_md5_finish_ret(&ctx, HashValue);    
   dEndTime = OS_TimeGet();
   
   /* Calculate execution time */   
   fExecTime = ((float)(dEndTime - dStartTime) / (float)OS_TICKS_PER_SECOND);
   fValue    = ((float)dCount / (1024.0 * 1024.0)) / fExecTime;   
   
   nR1 = (int)fValue;
   nR2 = ((fValue - (float)nR1) * 100.0);
   
   term_printf("| MD5          SW |    %3d.%02d |\r\n", nR1, nR2);
   OS_TimeDly(100);

} /* MD5Test */

/*************************************************************************/
/*  RIPEMD160Test                                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void RIPEMD160Test (void)
{
   uint32_t                 dStartTime;
   uint32_t                 dEndTime;
   uint32_t                 dCount;
   float                    fExecTime;
   float                    fValue;
   mbedtls_ripemd160_context ctx;
   uint8_t                   HashValue[20];
   int                      nR1, nR2;
   

   dCount     = 0;
   dStartTime = OS_TimeGet();
   dEndTime   = dStartTime + OS_MS_2_TICKS(1000);

   /* Start the HASH */
   mbedtls_ripemd160_init(&ctx);
   mbedtls_ripemd160_starts_ret(&ctx);

   do
   {
      /* Calculate the hash over the data */
      mbedtls_ripemd160_update_ret(&ctx, (uint8_t*)pTestData, HASH_DATA_CHUNK);
      dCount += HASH_DATA_CHUNK;
   } 
   while (OS_TimeGet() < dEndTime); 

   /* Get result */
   mbedtls_ripemd160_finish_ret(&ctx, HashValue);    
   dEndTime = OS_TimeGet();
   
   /* Calculate execution time */   
   fExecTime = ((float)(dEndTime - dStartTime) / (float)OS_TICKS_PER_SECOND);
   fValue    = ((float)dCount / (1024.0 * 1024.0)) / fExecTime;   
   
   nR1 = (int)fValue;
   nR2 = ((fValue - (float)nR1) * 100.0);
   
   term_printf("| RIPEMD160    SW |    %3d.%02d |\r\n", nR1, nR2);
   OS_TimeDly(100);

} /* RIPEMD160Test */

/*************************************************************************/
/*  SHA1Test                                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SHA1Test (void)
{
   uint32_t            dStartTime;
   uint32_t            dEndTime;
   uint32_t            dCount;
   float               fExecTime;
   float               fValue;
   mbedtls_sha1_context ctx;
   uint8_t              HashValue[20];
   int                 nR1, nR2;
   

   dCount     = 0;
   dStartTime = OS_TimeGet();
   dEndTime   = dStartTime + OS_MS_2_TICKS(1000);

   /* Start the HASH */
   mbedtls_sha1_init(&ctx);
   mbedtls_sha1_starts_ret(&ctx);

   do
   {
      /* Calculate the hash over the data */
      mbedtls_sha1_update_ret(&ctx, (uint8_t*)pTestData, HASH_DATA_CHUNK);
      dCount += HASH_DATA_CHUNK;
   } 
   while (OS_TimeGet() < dEndTime); 

   /* Get result */
   mbedtls_sha1_finish_ret(&ctx, HashValue);    
   dEndTime = OS_TimeGet();
   
   /* Calculate execution time */   
   fExecTime = ((float)(dEndTime - dStartTime) / (float)OS_TICKS_PER_SECOND);
   fValue    = ((float)dCount / (1024.0 * 1024.0)) / fExecTime;   
   
   nR1 = (int)fValue;
   nR2 = ((fValue - (float)nR1) * 100.0);
   
   term_printf("| SHA-1        SW |    %3d.%02d |\r\n", nR1, nR2);
   OS_TimeDly(100);

} /* SHA1Test */

/*************************************************************************/
/*  SHA256Test                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SHA256Test (void)
{
   uint32_t              dStartTime;
   uint32_t              dEndTime;
   uint32_t              dCount;
   float                 fExecTime;
   float                 fValue;
   mbedtls_sha256_context ctx;
   uint8_t                HashValue[32];
   int                   nR1, nR2;
   

   dCount     = 0;
   dStartTime = OS_TimeGet();
   dEndTime   = dStartTime + OS_MS_2_TICKS(1000);

   /* Start the HASH */
   mbedtls_sha256_init(&ctx);
   mbedtls_sha256_starts_ret(&ctx, 0);

   do
   {
      /* Calculate the hash over the data */
      mbedtls_sha256_update_ret(&ctx, (uint8_t*)pTestData, HASH_DATA_CHUNK);
      dCount += HASH_DATA_CHUNK;
   } 
   while (OS_TimeGet() < dEndTime); 

   /* Get result */
   mbedtls_sha256_finish_ret(&ctx, HashValue);    
   dEndTime = OS_TimeGet();
   
   /* Calculate execution time */   
   fExecTime = ((float)(dEndTime - dStartTime) / (float)OS_TICKS_PER_SECOND);
   fValue    = ((float)dCount / (1024.0 * 1024.0)) / fExecTime;   
   
   nR1 = (int)fValue;
   nR2 = ((fValue - (float)nR1) * 100.0);
   
   term_printf("| SHA-256      SW |    %3d.%02d |\r\n", nR1, nR2);
   OS_TimeDly(100);

} /* SHA256Test */

/*************************************************************************/
/*  SHA512Test                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SHA512Test (void)
{
   uint32_t              dStartTime;
   uint32_t              dEndTime;
   uint32_t              dCount;
   float                 fExecTime;
   float                 fValue;
   mbedtls_sha512_context ctx;
   uint8_t                HashValue[64];
   int                   nR1, nR2;
   

   dCount     = 0;
   dStartTime = OS_TimeGet();
   dEndTime   = dStartTime + OS_MS_2_TICKS(1000);

   /* Start the HASH */
   mbedtls_sha512_init(&ctx);
   mbedtls_sha512_starts_ret(&ctx, 0);

   do
   {
      /* Calculate the hash over the data */
      mbedtls_sha512_update_ret(&ctx, (uint8_t*)pTestData, HASH_DATA_CHUNK);
      dCount += HASH_DATA_CHUNK;
   } 
   while (OS_TimeGet() < dEndTime); 

   /* Get result */
   mbedtls_sha512_finish_ret(&ctx, HashValue);    
   dEndTime = OS_TimeGet();
   
   /* Calculate execution time */   
   fExecTime = ((float)(dEndTime - dStartTime) / (float)OS_TICKS_PER_SECOND);
   fValue    = ((float)dCount / (1024.0 * 1024.0)) / fExecTime;   
   
   nR1 = (int)fValue;
   nR2 = ((fValue - (float)nR1) * 100.0);
   
   term_printf("| SHA-512      SW |    %3d.%02d |\r\n", nR1, nR2);
   OS_TimeDly(100);

} /* SHA512Test */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  hash_Test                                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void hash_Test (void)
{
   term_printf("%s - Hash Benchmark\r\n", MBEDTLS_VERSION_STRING_FULL);
   term_printf("\r\n");
   term_printf("Compiler: gcc %d.%d.%d\r\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);   
   term_printf("System:   Processor speed = %d MHz\r\n", tal_CPUGetFrequencyCPU() / 1000000);    
   
   term_printf("\r\n");

   term_printf("+-----------------+-----------+\r\n");
   term_printf("| Algorithm       | Hash MB/s |\r\n");
   term_printf("+-----------------+-----------+\r\n");
   OS_TimeDly(100);

   MD5Test();
   RIPEMD160Test();
   SHA1Test();
   SHA256Test();
   SHA512Test();

   term_printf("+-----------------+-----------+\r\n");

   term_printf("\r\n");
   term_printf("Benchmark complete\r\n");
   term_printf("\r\n");
   
} /* hash_Test */

/*** EOF ***/
