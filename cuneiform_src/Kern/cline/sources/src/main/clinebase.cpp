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

#include <assert.h>
#include <stdlib.h>

# include "dpuma.h"
# include "lcline.h"
# include "time.h"

#include "compat_defs.h"

//������� �������
#define SNAP_ROOT_MAIN_DEBUG                "������� �������"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
Handle Root=NULL;
Handle MainDebug=NULL;
//Handle CLineDebug=NULL;
Handle MemStat=NULL;

CLINE_handle hcode;
int CLINE_Debug;
int CLINE_Debug_2;
int size_inv=0;
int size_event=0;
int size_cupoint=0;
int size_comp=0;
int size_line=0;

int size_class_inv=0;
int size_class_event=0;
int size_class_cupoint=0;
int size_class_comp=0;
int size_class_line=0;
int size_class_hline=0;

int len_inv_mas=10000;
int len_event_mas=1000;
int len_cupoint_mas=1000;
int len_comp_mas=100;
int len_line_mas=100;
int len_hline_mas=10;

CAllMem* pMyMem;
CInterval* pInvMem=NULL;
CEvent* pEventMem=NULL;
CCutPoint* pCupointMem=NULL;
CComponent* pCompMem=NULL;
CLine* pLineMem=NULL;
CHLine* pHLineMem=NULL;


LIST<CHLine>* pLCont;
CHLine* pMainCont;

/////////////////////////////////////////

BOOL InitData(void);
void DeleteData(void);

/////////////////////////////////////////
BOOL APIENTRY DllMain( HINSTANCE  hModule,
                        DWORD ul_reason_for_call,
                        LPVOID lpReserved )
{
    switch( ul_reason_for_call )
	{
    case DLL_PROCESS_ATTACH:
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CLINE_FUNC(Bool32) CLINE_Init(Word16 wHeightCode,HANDLE hStorage)
{
	LDPUMA_Init(0,NULL);

	pLCont=NULL;
	pMainCont=NULL;

	size_inv=sizeof(DInterval);
    size_event=sizeof(DEvent);
    size_cupoint=sizeof(DCutPoint);
    size_comp=sizeof(DComponent);
    size_line=sizeof(DLine);

	size_class_inv=sizeof(CInterval);
    size_class_event=sizeof(CEvent);
    size_class_cupoint=sizeof(CCutPoint);
    size_class_comp=sizeof(CComponent);
    size_class_line=sizeof(CLine);
	size_class_hline=sizeof(CHLine);

	_ASSERT(size_class_inv!=size_class_event && size_class_inv!=size_class_cupoint && size_class_inv!=size_class_comp && size_class_inv!=size_class_line && size_class_inv!=size_class_hline);
	_ASSERT(size_class_event!=size_class_cupoint && size_class_event!=size_class_comp && size_class_event!=size_class_line && size_class_event!=size_class_hline);
	_ASSERT(size_class_cupoint!=size_class_comp && size_class_cupoint!=size_class_line && size_class_cupoint!=size_class_hline);
	_ASSERT(size_class_comp!=size_class_line && size_class_comp!=size_class_hline);
	_ASSERT(size_class_line!=size_class_hline);

	if(!InitData())
		return FALSE;


	int time=clock();
	time=abs(time);
    hcode=(CLINE_handle)time;
	CLINE_Debug=0;
	CLINE_Debug_2=0;

//    LDPUMA_Registry(&MainDebug,SNAP_ROOT_MAIN_DEBUG,NULL);

	LDPUMA_Registry(&Root,"��������� �����",NULL);
	LDPUMA_RegistryHelp(Root,"<������������� ���. �����������> \
    \n ������� ������� ���������� �����.",FALSE);
	LDPUMA_RegVariable(Root,"CLINE_DEBUG",&CLINE_Debug,"int");
	LDPUMA_RegVariable(Root,"CLINE_DEBUG_WEAK",&CLINE_Debug_2,"int");

	LDPUMA_Registry(&MemStat,"������ ���������� �� ������",Root);
	LDPUMA_RegistryHelp(MemStat,"��������� ������ ���������� �� ������ � ���� clstat.res",FALSE);

 return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CLINE_FUNC(Bool32) CLINE_Done()
{
    DeleteData();
	LDPUMA_Done();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CLINE_FUNC(Word32) CLINE_GetReturnCode()
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
CLINE_FUNC(Int8 *) CLINE_GetReturnString(Word32 dwError)
{
   return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
CLINE_FUNC(Bool32) CLINE_GetExportData(Word32 dwType, void * pData)
{
	Bool32 rc = TRUE;

#define CASE_FUNCTION(a)	case CLINE_FN##a:	*(FN##a *)pData = a; break

   switch(dwType)
   {
	CASE_FUNCTION(CLINE_Reset);
	CASE_FUNCTION(CLINE_CreateContainer);
	CASE_FUNCTION(CLINE_DeleteContainer);
	CASE_FUNCTION(CLINE_GetFirstContainer);
	CASE_FUNCTION(CLINE_GetNextContainer);
	CASE_FUNCTION(CLINE_GetMainContainer);
	CASE_FUNCTION(CLINE_CleanContainer);
	CASE_FUNCTION(CLINE_GetLineCount);
	CASE_FUNCTION(CLINE_GetFirstLine);
	CASE_FUNCTION(CLINE_GetNextLine);
	CASE_FUNCTION(CLINE_AddNewLine);
	CASE_FUNCTION(CLINE_DelLine);
	CASE_FUNCTION(CLINE_DelAllLines);
	CASE_FUNCTION(CLINE_GetLineData);
	CASE_FUNCTION(CLINE_SetLineData);
	CASE_FUNCTION(CLINE_GetEventCount);
	CASE_FUNCTION(CLINE_GetFirstEvent);
	CASE_FUNCTION(CLINE_GetNextEvent);
	CASE_FUNCTION(CLINE_AddNewEvent);
	CASE_FUNCTION(CLINE_DelEvent);
	CASE_FUNCTION(CLINE_DelAllEvents);
	CASE_FUNCTION(CLINE_GetEventData);
	CASE_FUNCTION(CLINE_SetEventData);
	CASE_FUNCTION(CLINE_GetCutPointCount);
	CASE_FUNCTION(CLINE_GetFirstCutPoint);
	CASE_FUNCTION(CLINE_GetNextCutPoint);
	CASE_FUNCTION(CLINE_AddNewCutPoint);
	CASE_FUNCTION(CLINE_DelCutPoint);
	CASE_FUNCTION(CLINE_DelAllCutPoints);
	CASE_FUNCTION(CLINE_GetCutPointData);
	CASE_FUNCTION(CLINE_SetCutPointData);
	CASE_FUNCTION(CLINE_GetCompCount);
	CASE_FUNCTION(CLINE_GetFirstComp);
	CASE_FUNCTION(CLINE_GetNextComp);
	CASE_FUNCTION(CLINE_AddNewComp);
	CASE_FUNCTION(CLINE_DelComp);
	CASE_FUNCTION(CLINE_DelAllComps);
	CASE_FUNCTION(CLINE_GetCompData);
	CASE_FUNCTION(CLINE_SetCompData);
    CASE_FUNCTION(CLINE_GetEventInvCount);
	CASE_FUNCTION(CLINE_GetFirstEventInv);
	CASE_FUNCTION(CLINE_GetNextEventInv);
	CASE_FUNCTION(CLINE_AddNewEventInv);
	CASE_FUNCTION(CLINE_DelEventInv);
	CASE_FUNCTION(CLINE_DelAllEventInvs);
	CASE_FUNCTION(CLINE_GetEventInvData);
	CASE_FUNCTION(CLINE_SetEventInvData);
    CASE_FUNCTION(CLINE_GetCutPointInvCount);
	CASE_FUNCTION(CLINE_GetFirstCutPointInv);
	CASE_FUNCTION(CLINE_GetNextCutPointInv);
	CASE_FUNCTION(CLINE_AddNewCutPointInv);
	CASE_FUNCTION(CLINE_DelCutPointInv);
	CASE_FUNCTION(CLINE_DelAllCutPointInvs);
	CASE_FUNCTION(CLINE_GetCutPointInvData);
	CASE_FUNCTION(CLINE_SetCutPointInvData);
	CASE_FUNCTION(CLINE_CopyData);

	default:
		*(Handle *)pData = NULL;
		rc = FALSE;
	}
return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
CLINE_FUNC(Bool32) CLINE_SetImportData(Word32 dwType, void * pData)
{
   return FALSE;
}

CLINE_FUNC(Word32) CLINE_SetReturnCode(Word32 rc)
{
   return (Word32)(0);
}

