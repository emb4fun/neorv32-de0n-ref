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
#define __ECDSA_TEST_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <string.h>
#include "tal.h"
#include "terminal.h"

#include "mbedtls/version.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all extern Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static uint8_t Hash[512];
static uint8_t Signature[512];

static mbedtls_pk_context       pk;
static mbedtls_entropy_context  entropy;
static mbedtls_ctr_drbg_context ctr_drbg;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  ECDSATest                                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void ECDSATest (mbedtls_ecp_group_id grp_id)
{
   int      nErr;
   size_t    sig_len;
   uint32_t dGenStartTime;
   uint32_t dGenTime;
   uint32_t dGenMem;
   uint32_t dSignStartTime;
   uint32_t dSigTime;
   uint32_t dSigMem;
   uint32_t dVerifyStartTime;
   uint32_t dVerifyTime;
   uint32_t dVerifyMem;
   char     *name;
   
   const char *pers = "gen_key";

   /*
    * Prepare key generation
    */
   mbedtls_pk_init(&pk);
   mbedtls_ctr_drbg_init(&ctr_drbg);
   mbedtls_entropy_init(&entropy);

   nErr =  mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *)pers, strlen( pers ));
   if (nErr != 0)
   {
      term_printf("Error mbedtls_ctr_drbg_seed -0x%04x\r\n", -nErr );
      goto Cleanup;
   }

   /*
    * Generate the key
    */
   tal_MEMClrUsedRawMemoryMax(XM_ID_TLS);
   dGenStartTime     = OS_TimeGet();
    
   nErr = mbedtls_pk_setup(&pk, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
   if (nErr != 0)
   {
      term_printf("Error mbedtls_pk_setup -0x%04x\r\n", -nErr);
      goto Cleanup;
   }

   nErr = mbedtls_ecp_gen_key(grp_id, mbedtls_pk_ec(pk), mbedtls_ctr_drbg_random, &ctr_drbg);
   if (nErr != 0)
   {
      term_printf("Error mbedtls_ecp_gen_key -0x%04x\r\n", -nErr);
      goto Cleanup;
   }

   dGenTime = OS_TimeGet() - dGenStartTime;
   dGenMem  = tal_MEMGetUsedRawMemoryMax(XM_ID_TLS);  


   /* 
    * Generate hash 
    */
   memset(Hash, 0x2A, sizeof(Hash));
   memset(Signature, 0x00, sizeof(Signature));


   /* 
    * Sign 
    */
   tal_MEMClrUsedRawMemoryMax(XM_ID_TLS);
   dSignStartTime    = OS_TimeGet();
   nErr = mbedtls_pk_sign(&pk, MBEDTLS_MD_SHA256, Hash, 0, Signature, &sig_len,
                          mbedtls_ctr_drbg_random, &ctr_drbg);
   dSigTime = OS_TimeGet() - dSignStartTime;
   dSigMem  = tal_MEMGetUsedRawMemoryMax(XM_ID_TLS);  


//   term_printf("BitLen %d\r\n", mbedtls_pk_get_bitlen(&pk));

//   term_printf("SigLen %d\r\n", sig_len);


   /* 
    * Verify 
    */
   tal_MEMClrUsedRawMemoryMax(XM_ID_TLS);
   dVerifyStartTime  = OS_TimeGet();
   nErr = mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, Hash, 0, Signature, sig_len);
   dVerifyTime = OS_TimeGet() - dVerifyStartTime;
   dVerifyMem  = tal_MEMGetUsedRawMemoryMax(XM_ID_TLS);  

   name = (char*)mbedtls_ecp_curve_info_from_grp_id(grp_id)->name;
   term_printf("| %-16s |  %5d |   %5d |  %4d |  %5d |   %5d |   %5d |\r\n", name, dGenTime, dGenMem, dSigTime, dSigMem, dVerifyTime, dVerifyMem);
   OS_TimeDly(100);


Cleanup:

   mbedtls_pk_free(&pk);
   mbedtls_ctr_drbg_free(&ctr_drbg);
   mbedtls_entropy_free(&entropy);

} /* ECDSATest */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  ecdsa_Test                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void ecdsa_Test (void)
{
   term_printf("%s - ECDSA Gen, Sign and Verify Benchmark\r\n", MBEDTLS_VERSION_STRING_FULL);
   term_printf("\r\n");
   term_printf("Compiler: gcc %d.%d.%d\r\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);   
   term_printf("System:   Processor speed = %d MHz\r\n", tal_CPUGetFrequencyCPU() / 1000000);    
   term_printf("Window:   %d\r\n", MBEDTLS_ECP_WINDOW_SIZE);    
   
   term_printf("\r\n");

   term_printf("+------------------+--------+---------+-------+--------+---------+---------+\r\n");
   term_printf("|                  |    Gen |   Gen   |  Sign |  Sign  |  Verify |  Verify |\r\n");
   term_printf("| Curve            |     ms |   bytes |    ms |  bytes |      ms |   bytes |\r\n");
   term_printf("+------------------+--------+---------+-------+--------+---------+---------+\r\n");
   OS_TimeDly(100);

   ECDSATest(MBEDTLS_ECP_DP_SECP192R1);
   //ECDSATest(MBEDTLS_ECP_DP_SECP192K1);
   ECDSATest(MBEDTLS_ECP_DP_SECP224R1);
   //ECDSATest(MBEDTLS_ECP_DP_SECP224K1);
   ECDSATest(MBEDTLS_ECP_DP_SECP256R1);
   //ECDSATest(MBEDTLS_ECP_DP_SECP256K1);
   ECDSATest(MBEDTLS_ECP_DP_SECP384R1);
   ECDSATest(MBEDTLS_ECP_DP_SECP521R1);

   //ECDSATest(MBEDTLS_ECP_DP_BP256R1);
   //ECDSATest(MBEDTLS_ECP_DP_BP384R1);
   //ECDSATest(MBEDTLS_ECP_DP_BP512R1);
   
   term_printf("+------------------+--------+---------+-------+--------+---------+---------+\r\n");

   term_printf("\r\n");
   term_printf("Benchmark complete\r\n");
   term_printf("\r\n");

} /* ecdsa_Test */

/*** EOF ***/
