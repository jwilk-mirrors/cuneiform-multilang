/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

      * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
        ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
        ����� �� ��������.
      * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
        ������ ����������, ������������ ��� ���������������, ������ �����������
        ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
        ����������� ����� �� ��������.
      * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
        ���� ������������ � �������� �������� ��������� �/��� �����������
        ���������, ���������� �� ���� ��, ��� ���������������� �����������
        ����������.

��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
������������� ������, ��������� � �������������� ��� ���������� ����������
������������� ������������� ��������� (������� ������ ������, ��� ������,
������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
�� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
�������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// RCUTP.cpp : Defines the entry point for the DLL application.
//
/*#include "StdAfx.h"*/
#include <stdlib.h>
#include "resource.h"
#include "rcutp.h"
#include "rcutp_prot.h"
#define __RCUTP_EXTERN__

#include "dpuma.h"
#include "dsnap.h"

#include "compat_defs.h"

//--------------------------GLOBAL VARIABLES------------------------
Int16      minrow,bbs1,bbs2,bbs3,bbs4,Nb1,Nb2,Nb3;
Word8      language;

HINSTANCE  ghInst = NULL;
Word16     gwHeightRC = 0;
Word16     gwLowRC = 0;
Handle     ghStorage = NULL;
Handle     hDebugRoot, hDebugMy,hDebugCutP;

///////////////////////////////////////////////////////////////
RCUTP_FUNC(Bool32) RCUTP_Init(Word16 wHeightCode,Handle hStorage)
{
	LDPUMA_Init(0,NULL);
	LDPUMA_Registry(&hDebugRoot,SNAP_ROOT_CONVERTERS,NULL);
	LDPUMA_Registry(&hDebugMy,"������� ����� ����������",hDebugRoot);

	LDPUMA_Registry(&hDebugCutP,"�������� ����� ����������",hDebugMy);
	LDPUMA_RegistryHelp(hDebugCutP,"��� ����� ������������� ��� ������ ����� ����������",FALSE);

	gwHeightRC = wHeightCode;

 return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Bool32) RCUTP_Done()
{
	LDPUMA_Done();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Word32) RCUTP_GetReturnCode()
{
Word32 rc = 0;
	if((gwLowRC - IDS_ERR_NO)>0)
		rc = (Word32)(gwHeightRC<<16)|(gwLowRC - IDS_ERR_NO);

return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Int8 *) RCUTP_GetReturnString(Word32 dwError)
{
	Word16 rc = (Word16)(dwError & 0xFFFF) + IDS_ERR_NO;
	static Int8 szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_ERR_NOTIMPLEMENT;

	if( rc >= IDS_ERR_NO )
		LoadString(ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return szBuffer;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Bool32) RCUTP_GetExportData(Word32 dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;

#define CASE_FUNCTION(a)	case RCUTP_FN##a:	*(FN##a *)pData = a; break
#define CASE_DATA(a,b,c)	case a: *(b *)pData = c; break

	switch(dwType)
 {
		CASE_FUNCTION(RCUTP_CutPoints);
		CASE_FUNCTION(RCUTP_SetBL_for_CutPoints);

  default:
        		*(Handle *)pData = NULL;
		        gwLowRC = IDS_ERR_NOTIMPLEMENT;
		        rc = FALSE;
	}

#undef CASE_DATA
#undef CASE_FUNCTION

return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Bool32) RCUTP_SetImportData(Word32 dwType, void * pData)
{
	Bool32 rc = TRUE;
return rc;
}

void SetReturnCode_rcutp(Word16 rc)
{
	gwLowRC = rc;
}

Word16 GetReturnCode_rcutp()
{
	return gwLowRC;
}
