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
 * File: SigmaWTG.c - The main program for Sigma WTG Service
 *       This is the program which controls the PC-Endpoint and all the required 
 *	 control agents for Testbed STAs. This program runs as service and starts
 *	 during the system bootup. It restarts any WTG component(e.g. wfa_ca) 
 *       in case of abnormal termination
 *
 * Revision History:
 *       2008/08/12  -- Initially created by anv
 *       2010/06/30  -- Reformat the file for maintainability by coding guidelines
 */

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "SigmaWTG.h"

struct WTG_Thread threadPool[MAX_THREAD];
int iLogging=0;
FILE *serviceLogFile;

int main(int argc,char **argv)
{
    FILE *fp;
    char Line[256];
    char STAIPAddress[256],sControlPortName[256],portNumber[256],sDUTPort[256],sCommand[1024],sPath[512];
    char sValue[512];
    int i=0;

    memset(Line,'\0',sizeof(Line));
    memset(STAIPAddress,'\0',sizeof(STAIPAddress));
    memset(sControlPortName,'\0',sizeof(sControlPortName));
    memset(sPath,'\0',sizeof(sPath));
    memset(sValue,'\0',sizeof(sValue));
    memset(sDUTPort,'\0',sizeof(sDUTPort));
	
    serviceLogFile=fopen("/var/log/WTG-Sigma.log","w");
    int iThreadCounter=0;
    if (argc < 2)
    {
        printf("\n Usage : ./SigmaWTG Testbed STA Info File> \
        \n    eg : ./SigmaWTG SigmaWTG.conf \n");
    } 
    else 
    {
        if ((fp=fopen(argv[1],"r"))) 
        {
            while (fgets(Line,255,fp)) 
            {
                //system("./wfa_ca eth0 9000");
                if (!isLineComment(Line))
                {
                   if ( *sPath && *sControlPortName && 
                 getParameterValue(Line,"control_ip",STAIPAddress) &&
                 getParameterValue(Line,"control_agent_port",portNumber) && 
                 getParameterValue(Line,"endpoint_port_number", sDUTPort)) 
                   {
                      if(iLogging)
                         sprintf(sCommand,"%s %s %s %s %s %s/%s_%s.%s",
                             WFA_CA,sControlPortName,portNumber,STAIPAddress,
                             sDUTPort,sPath,STAIPAddress,portNumber,"log");
                      else
                         sprintf(sCommand,"%s %s %s %s %s",
                             WFA_CA,sControlPortName,portNumber,STAIPAddress,sDUTPort);
                      /* Start Control Agent for Testbed STAs */
                      threadPool[iThreadCounter].thread_id=pthread_create(&threadPool[iThreadCounter].thr, &threadPool[iThreadCounter].attr,startControlAgent,(void *)sCommand);
                      sleep(2);
                      iThreadCounter++;
                   } 
                   else if ( *sPath && *sControlPortName && getParameterValue(Line,"wtg_control_port_number",sValue)) 
                   {
                      if(iLogging)		
		        sprintf(sCommand,"%s %s %s  %s",WFA_DUT,"lo","8000","/var/log/WTG.log");
                      else
                        sprintf(sCommand,"%s %s %s",WFA_DUT,"lo","8000");

                      /* Start Traffic Generator of PC-EndPoint */
                      threadPool[iThreadCounter].thread_id=pthread_create(&threadPool[iThreadCounter].thr, &threadPool[iThreadCounter].attr,startTG,(void *)sCommand);
                      sleep(2);
                      iThreadCounter++;

                      if(iLogging)
                        sprintf(sCommand,"%s %s %s %s %s %s/%s_%s.%s",WFA_CA,"lo",sValue,"127.0.0.1", "8000",sPath,"WTG",sValue,"log");
                      else
                        sprintf(sCommand,"%s %s %s %s %s",WFA_CA,sControlPortName,sValue,"127.0.0.1","8000");

                      /* Start Control Agent of PC-EndPoint */
                      threadPool[iThreadCounter].thread_id=pthread_create(&threadPool[iThreadCounter].thr, &threadPool[iThreadCounter].attr,startControlAgent,sCommand);
                      sleep(2);
                      iThreadCounter++;
                  }
                  else if (getParameterValue(Line,"PATH",sPath)) 
                  {
                      wfa_print("\n PATH = %s",sPath);
                  }
                  else if (getParameterValue(Line,"control_port_name",sControlPortName)) 
                  {
                      wfa_print("\n Control Port Name = %s",sControlPortName);
                  }
                  else if (getParameterValue(Line,"logging",sValue)) 
                  {
                      if (strstr(sValue,"1")) 
                      {
                         iLogging=1;
                      }
                  } 
                  else 
                  {
                      wfa_print("\n Invalid Line = %s",Line);
                  }
              }
			
              memset(sValue,'\0',sizeof(sValue));
	      memset(sCommand,'\0',sizeof(sCommand));
          }
      } 
      else 
      {
          wfa_print("\n Invalid File Name - %s \n",argv[1]);
          printf("\n Invalid File Name - %s \n",argv[1]);
      }
   }

   fflush(serviceLogFile);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);
   /* Waiting for all threads to be completed */
   for(i=0;i<iThreadCounter;i++) 
   {
       wfa_print("\n Waiting for Thread...");
       pthread_join(threadPool[i].thr,NULL);
   }

   return 0;
}

int getParameterValue(char *source,const char *Parameter, char *Value)
{
    char *tmp;
    char *retString=NULL;
    char sourceStr[256];
    int iRet=0;
    if(!Parameter || !source) 
       return 0;

    strcpy(sourceStr,source);
    tmp=strstr(sourceStr,Parameter);
    if (tmp)
    {
        retString=strtok((tmp+strlen(Parameter)+1),";\n ");
        if(retString) 
        {
           strcpy(Value,retString);
           iRet=1;
        }
    }

    return iRet;
}

int isLineComment (char *Line)
{
    if (strlen(Line)<5) return 1;
    while (Line)
    {
        if (*Line != ' ' && *Line != '#')
           return 0;
        if (*Line == '#')
           return 1;

	Line++;
    }

    return 1;
}

void* startControlAgent(void *commandLine) 
{
    char sCommand[1024];
    sprintf(sCommand,"%s",(char *)commandLine);
    wfa_print("\n Starting Control Agent -  %s ",sCommand);
    while(1)
    {
        system(sCommand);
        sleep(10);
        wfa_print("\n Restarting Control Agent -  %s",sCommand);
    }
}

void* startTG(void *commandLine) 
{
    char sCommand[1024];
    sprintf(sCommand,"%s",(char *)commandLine);
    wfa_print("\n Starting WTG -  %s",(char *)sCommand);

    while(1)
    {
       system(sCommand);
       sleep(5);
       wfa_print("\n Restarting WTG -  %s",sCommand);
    }
}

void wfa_print( char *fmt, ... ) 
{
   va_list args;
   struct timeval tv;
   time_t curtime;
   char sTimeStamp[256];
   gettimeofday(&tv, NULL); 
   curtime=tv.tv_sec;
   strftime(sTimeStamp,30,"%m-%d-%Y  %T",localtime(&curtime));

   if(serviceLogFile)
   {
       va_start( args, fmt );
       fprintf( serviceLogFile," [ %s ] ",sTimeStamp );
       vfprintf(serviceLogFile, fmt, args );
       va_end( args );
       fflush(serviceLogFile);
   }
} 
