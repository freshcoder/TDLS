/****************************************************************************
 *  (c) Copyright 2007 Wi-Fi Alliance.  All Rights Reserved
 *
 *  Author - Ankur Vachhani (avachhani@wi-fi.org)
 *
 *  LICENSE
 *
 * License is granted only to Wi-Fi Alliance members and designated
 * contractors (“Authorized Licensees”).  Authorized Licensees are granted
 * the non-exclusive, worldwide, limited right to use, copy, import, export
 * and distribute this software:
 * (i) solely for noncommercial applications and solely for testing Wi-Fi
 * equipment; and
 * (ii) solely for the purpose of embedding the software into Authorized
 * Licensee’s proprietary equipment and software products for distribution to
 * its customers under a license with at least the same restrictions as
 * contained in this License, including, without limitation, the disclaimer of
 * warranty and limitation of liability, below.  The distribution rights
 * granted in clause (ii), above, include distribution to third party 
 * companies who will redistribute the Authorized Licensee’s product to their 
 * customers with or without such third party’s private label. Other than 
 * expressly granted herein, this License is not transferable or sublicensable,
 * and it does not extend to and may not be used with non-Wi-Fi applications. 
 * Wi-Fi Alliance reserves all rights not expressly granted herein. 
 * 
 * Except as specifically set forth above, commercial derivative works of
 * this software or applications that use the Wi-Fi scripts generated by this
 * software are NOT AUTHORIZED without specific prior written permission from
 * Wi-Fi Alliance. Non-Commercial derivative works of this software for 
 * internal use are authorized and are limited by the same restrictions; 
 * provided, however, that the Authorized Licensee shall provide Wi-Fi Alliance
 * with a copy of such derivative works under a perpetual, payment-free license
 * to use, modify, and distribute such derivative works for purposes of testing
 * Wi-Fi equipment.
 * Neither the name of the author nor "Wi-Fi Alliance" may be used to endorse
 * or promote products that are derived from or that use this software without
 * specific prior written permission from Wi-Fi Alliance.
 *
 * THIS SOFTWARE IS PROVIDED BY WI-FI ALLIANCE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE,
 * ARE DISCLAIMED. IN NO EVENT SHALL WI-FI ALLIANCE BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, THE COST OF PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ****************************************************************************
 */
 /*
 * File: SigmaWTG.h - The header file for SigmaWTG.c
 *
 * Revision History:
 *       2008/08/12  -- Initially created by anv
 */


#define MAX_THREAD 10

/* Control Agent binary name */
#define WFA_CA "/usr/bin/wfa_ca"

/* WFA DUT binary name */
#define WFA_DUT "/usr/bin/wfa_dut"

void* startControlAgent(void *commandLine); 
void *startTG(void *commandLine);
void wfa_print( char *fmt, ... );
int getParameterValue(char *source,const char *Parameter, char *Value);
int isLineComment (char *Line);


struct WTG_Thread {
	pthread_t thr;
	pthread_attr_t attr;
	int thread_id;
	char command[256];
};
