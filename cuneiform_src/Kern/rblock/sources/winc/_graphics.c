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

#include "layout.h"
#include "extract.h"

#include "status.h"

#include "graphics.h"

#include "dpuma.h"
#include "pumadef.h"

////////////////////////////////////////////////

extern void EnableDebug(void);
////////////////////////////////////////////////

void _setvideomode (int mode){};
void _clearscreen (int mode)
{
	LDPUMA_DeleteRects(NULL,PUMA_MODULE_RBLOCK);
	LDPUMA_DeleteLines(NULL,PUMA_MODULE_RBLOCK);
	LDPUMA_DeleteStrings(NULL,PUMA_MODULE_RBLOCK);
	//LDPUMA_UpdateView(NULL);
};
static Word32 swColor = 0;
void _setcolor (int color)
{
	#define __RGB__(r,g,b)          ((Word32)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
	Word32 clr[16] =
	{ // 16 ������ � �������, �������� DOS (see MENUCOLOR :) )
    __RGB__(0,0,0),          // 0  ������    _COLOR_BLACK
    __RGB__(0,0,127),        // 1  �����-�����     _COLOR_DBLUE
    __RGB__(0,127,0),        // 2  �����-�������    _COLOR_DGREEN
    __RGB__(0,127,127),      // 3  �����-��������� (dark cyan)    _COLOR_DCYAN
    __RGB__(127,0,0),        // 4  �����-�������    _COLOR_DRED
    __RGB__(127,0,127),      // 5  �����-������� (/���������/����������)    _COLOR_DMAGENTA
    __RGB__(0x99,0x66,00),   // 6  ����������    _COLOR_BROWN
    __RGB__(0xCC,0xCC,0xCC), // 7  ������-�����    _COLOR_GRAY
    __RGB__(0x99,0x99,0x99), // 8  �����-�����     _COLOR_DGRAY
    __RGB__(0,0,255),        // 9  �����    _COLOR_BLUE
    __RGB__(0,255,0),        // 10 ����-�������    _COLOR_GREEN
    __RGB__(0,255,255),      // 11 ������-���������    _COLOR_CYAN
    __RGB__(255,0,0),        // 12 ����-�������    _COLOR_RED
    __RGB__(255,0,255),      // 13 ��������� (/�������)    _COLOR_MAGENTA
    __RGB__(255,255,0),      // 14 ������    _COLOR_YELLOW
    __RGB__(255,255,255)     // 15 �����    _COLOR_WHITE
    // -- �� 16.10.2001 --
    //		__RGB__(0,0,0),// 0 ������
    //		__RGB__(0,0,127),// 1 �����
    //		__RGB__(0,127,0),// 2 �������
    //		__RGB__(64,64,127),// 3 �������
    //		__RGB__(127,0,0),// 4 �������
    //		__RGB__(127,64,64),// 5 �������
    //		__RGB__(64,127,64),// 6 ����������
    //		__RGB__(127,127,127),// 7 �����
    //		__RGB__(0,0,255),// 1 �����
    //		__RGB__(0,255,0),// 2 �������
    //		__RGB__(127,127,255),// 3 �������
    //		__RGB__(255,0,0),// 4 �������
    //		__RGB__(255,127,127),// 5 �������
    //		__RGB__(127,255,127),// 6 ����������
    //		__RGB__(255,255,0),//15 �����
    //		__RGB__(255,0,255)//15 �����
    // -- �� 16.10.2001 --
	};
	swColor = clr[color];
	#undef __RGB__
};
void _rectangle (int t,int left,int top,int right,int bottom)
{
	Rect16 rect = {left,top,right,bottom};
	switch(t)
	{
	case _GFILLINTERIOR: // ��������� �������������
		LDPUMA_DrawRect(NULL,&rect,0,swColor,0,PUMA_MODULE_RBLOCK);
		break;
	case _GBORDER: // ������� � 16/100 ������� �����������
		LDPUMA_DrawRect(NULL,&rect,0,swColor,-16,PUMA_MODULE_RBLOCK);
		break;
//	case _G...: // ������� ����� -- ���� ������ ������
//		LDPUMA_DrawRect(NULL,&rect,0,swColor,1,PUMA_MODULE_RBLOCK);
//		break;
	default:
		LDPUMA_DrawRect(NULL,&rect,0,swColor,-16,PUMA_MODULE_RBLOCK);
	}
};
void _getfillmask (char * OldMask){};
void _setfillmask (char * OldMask){};
static Point16 sBeg = {0,0};
void _moveto (int x,int y)
{
	sBeg.x = x;
	sBeg.y = y;
};
void _lineto (int x,int y)
{
	Point16 end = {x,y};
	LDPUMA_DrawLine(NULL,&sBeg,&end,0,swColor,-16,PUMA_MODULE_RBLOCK);
	sBeg = end;
};
void _setlinestyle (int style){};

#ifndef WIN32
typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
#endif

void _gettextsettings (struct textsettings *ts)
{
	LPBITMAPINFOHEADER lp = LDPUMA_GetDIBptr(NULL);
	if(lp)
	{
		ts->height = lp->biHeight;
		ts->width = lp->biWidth;
	}
};

static Word32 swAlign = 0;
void _settextalign (int right,int top)
{
	swAlign = 0;
	switch(right)
	{
		case _RIGHT:
			swAlign |= TA_RIGHT;
			break;
		case _TOP:
			swAlign |= TA_TOP;
			break;
		case _CENTER:
			swAlign |= TA_CENTER;
			break;
		case _HALF:
			swAlign |= TA_CENTER;
			break;
	}
	switch(top)
	{
		case _RIGHT:
			swAlign |= TA_RIGHT;
			break;
		case _TOP:
			swAlign |= TA_TOP;
			break;
		case _CENTER:
			swAlign |= TA_CENTER;
			break;
		case _HALF:
			swAlign |= TA_CENTER;
			break;
	}
};
void _setcharsize (int n1, int n2){};
void _grtext (int x, int y, const char * text)
{
	Point16 p = {x,y};
	LDPUMA_DrawString(NULL,&p,text,swAlign,swColor,120/*12*/,PUMA_MODULE_RBLOCK);
};
unsigned _getlinestyle (){return 0;};
void break_point_data (char * c){};
void write_prot () {};
BOOL snapAutofont(BYTE a){ a=a; return TRUE; } // Andrew Theer

int LT_Getch (void)
{
	Word32 rc;
	rc = LDPUMA_WaitUserInput(NULL,NULL);
	EnableDebug();
	return rc;
}

////////////////////////////////////////////////
