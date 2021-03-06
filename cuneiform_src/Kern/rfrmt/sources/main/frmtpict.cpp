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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 05.04.99
//
// FrmtPict.cpp
// Source file for Device-Independent Bitmap (DIB) API.  Provides
// the following functions:
// GetPictCount()      - Return Picture count
// WritePict()         - Write  Picture

// ============================================================================
//#include "afx.h"
//#include <afxwin.h>
#include "stdafx.h"

#include <math.h>
/*#include <io.h>
#include <direct.h>*/

#include "cpage.h"
#include "cpagetyps.h"
#include "rtfedwrite.h"
#include "frmtpict.h"
#include "dpuma.h"
#include "pumadef.h"
#include "ctdib.h"
#include "ctiimage.h"
#include "frmtdibapi.h"
#include "criimage.h"
#include "sys_prog.h"
#include "ced.h"
#include "rfrmt_prot.h"

#include "minmax.h"

#define  USE_NONE              0x0040   // no formatting

extern   Word32 FlagMode;
extern   Word32 RtfWriteMode;
extern   POINT  TemplateOffset;

extern	  CHAR   RtfFileName[MAX_PATH];
//BOOL SaveMetafile(CString * str, /*CIMAGE*/BITMAPINFOHEADER * lpDIB);

/*
* Dib Header Marker - used in writing DIBs to files
*/
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')


//==============   ����������� ���-�� ������ �� ��������  ======================
Word32 GetPictCount(void)
{
	Word32 PictCount = 0;
	Word32 NumberPage = CPAGE_GetCurrentPage();
	Handle h_Page = CPAGE_GetHandlePage(NumberPage);

	Handle h_Pict = CPAGE_PictureGetFirst(h_Page);
	while(h_Pict)
	{
		PictCount++;
		h_Pict = CPAGE_PictureGetNext(h_Page,h_Pict);
	}
	return PictCount;
}

//=====================     ������ ��������     ===================================
BYTE GetPictRect( Word32 NumberPict , Rect16* RectPict,Word32* UserNumber )
{
	Word32  PictCount  = 0;
	Point32 Lr = {0};
	Point32 Wh = {0};
	Word32  NumberPage = CPAGE_GetCurrentPage();
	Handle  h_Page     = CPAGE_GetHandlePage(NumberPage);
	Handle  h_Pict     = CPAGE_PictureGetFirst(h_Page);

	while( h_Pict && PictCount < NumberPict )
	{
		PictCount++;
		h_Pict = CPAGE_PictureGetNext( h_Page,h_Pict );
	}

	if(!h_Pict)
		return FALSE;


	*UserNumber = (Word32)CPAGE_GetBlockUserNum( h_Page, h_Pict  );


	if(CPAGE_PictureGetPlace (h_Page, h_Pict, 0, &Lr, &Wh))
	{
		RectPict->left   = (Int16)( Lr.x - TemplateOffset.x);
		RectPict->right  = (Int16)( Lr.x - TemplateOffset.x + Wh.x );
		RectPict->top    = (Int16)( Lr.y - TemplateOffset.y);
		RectPict->bottom = (Int16)( Lr.y - TemplateOffset.y + Wh.y );
	}
	return TRUE;
}

//**************************** ������ ������ ************************************
BOOL WritePict( Word32 IndexPict,RtfSectorInfo* SectorInfo /*, CString* PictString*/, BOOL OutPutTypeFrame)
{
	Word32        PictNumber    = 0;
//	int           Realx,Realy;
	Point32       RtfLt = {0};
	CPAGE_PICTURE pict = {0};


	LDPUMA_Skip(hTest);

#ifdef EdWrite
	Handle 	hParagraph=NULL;
	Handle 	hString=NULL;

	EDSIZE   pictSize;
	EDSIZE   pictGoal;
	EDRECT   indent;
	EDBOX    playout;
	EDRECT   slayout;
	EDSIZE   interval;
	EDBOX    EdFragmRect;
	letterEx Letter;
	Handle   hPrevObject;
#endif

	Word32   NumberPage    = CPAGE_GetCurrentPage();
	Handle   h_Page        = CPAGE_GetHandlePage(NumberPage);
	Handle   h_Pict        = CPAGE_PictureGetFirst(h_Page);
//	CString  str;
/*
	if(RtfWriteMode)
		PictString->Empty();
*/
	while( h_Pict )
	{
		if( ++PictNumber > IndexPict )
			break;
		h_Pict = CPAGE_PictureGetNext(h_Page,h_Pict);
	}

	if(!h_Pict)
		return 0;

	PAGEINFO pinfo = {0};
	if(GetPageInfo(h_Page,&pinfo))
	{
		CIMAGE_InfoDataInGet in = {0};
		CIMAGEBITMAPINFOHEADER image_info = {0};
		Word32 nSize = 0;
		Point32 Lr = {0};
		Point32 Wh = {0};
		Point32 PLr = {0};
		Point32 LrN = {0};
		Point32 WhN = {0};
		WORD    FrameOffset=0;

		if(CIMAGE_GetImageInfo((PWord8)pinfo.szImageName,&image_info)==FALSE)
			return 0;
		CPAGE_PictureGetPlace (h_Page, h_Pict, 0, &Lr, &Wh);
		CPAGE_PictureGetPlace (h_Page, h_Pict,-pinfo.Incline2048, &LrN, &WhN);

		Lr.x -= TemplateOffset.x;
		Lr.y -= TemplateOffset.y;

		FrameOffset = abs(WhN.x - Wh.x);
		if(Lr.x<0)
			FrameOffset += abs(Lr.x);
/*
 	if(CPAGE_GetBlockData(h_Page,h_Pict,TYPE_CPAGE_PICTURE,&pict,sizeof(pict))==sizeof(pict))
	 {
		 for(Word32 i = 0; i<pict.Number;i++)
		 {
				RtfLt.x = pict.Corner[i].x;
				RtfLt.y = pict.Corner[i].y;
			}
		}
*/

/*		if(RtfWriteMode)
		{
			if(FlagMode & USE_NONE)
				str.Format("\\marglsxn1800\\margrsxn1800\\margtsxn1440\\margbsxn1440\\headery1440\\footery1440\\sbknone\\pard\\f3\\fs6\\par\\par\\fi0");
			else
				if( SectorInfo->CountFragments == 1 )
				{
					Realx = MAX(0,(int)(Lr.x*Twips));
					str.Format("\\marglsxn%i\\margrsxn1800\\margtsxn360\\margbsxn360\\headery360\\footery360\\sbknone\\pard\\f3\\fs6\\par\\par\\fi0",Realx);
				}
				else
				if(SectorInfo->FlagInColumn==TRUE)
				{
 					Realx = SectorInfo->OffsetFromColumn.x;
					Realy = SectorInfo->OffsetFromColumn.y;
					if( SectorInfo->FlagFictiveParagraph)
					{
						str.Format("\\pard\\fs6\\par\\par{\\shp{\\*\\shpinst\\shpleft%i\\shptop%i\\shpright%i\\shpbottom%i\\shpfhdr0\\shpbxcolumn\\shpbypara\\shpwr2\\shpwrk0\\shpfblwtxt1\\shpz0\\shplockanchor{\\sp{\\sn shapeType}{\\sv 75}}{\\sp{\\sn fFlipH}{\\sv 0}}{\\sp{\\sn fFlipV}{\\sv 0}}{\\sp{\\sn pib}{\\sv ",Realx,Realy,Realx + int(Wh.x*Twips),Realy + int(Wh.y*Twips));
						SectorInfo->FlagFictiveParagraph = FALSE;
					}
					else
						str.Format("\\pard\\fs6\\par{\\shp{\\*\\shpinst\\shpleft%i\\shptop%i\\shpright%i\\shpbottom%i\\shpfhdr0\\shpbxcolumn\\shpbypara\\shpwr2\\shpwrk0\\shpfblwtxt1\\shpz0\\shplockanchor{\\sp{\\sn shapeType}{\\sv 75}}{\\sp{\\sn fFlipH}{\\sv 0}}{\\sp{\\sn fFlipV}{\\sv 0}}{\\sp{\\sn pib}{\\sv ",Realx,Realy,Realx + int(Wh.x*Twips),Realy + int(Wh.y*Twips));
				}
 				else
	 			{
					Realx = (int)(Lr.x*Twips - SectorInfo->Offset.x);
					Realy = (int)(Lr.y*Twips - SectorInfo->Offset.y);
					str.Format("{\\pard\\s0\\pvpara\\phmrg\\posx%i\\posy%i\\absh0\\absw%i\\abslock1\\dxfrtext180\\dfrmtxtx0\\dfrmtxty0\\ql\\plain\\fs6\\f3\\fs0\\fi0",
							Realx,Realy,(int)(MAX(0, Wh.x-FrameOffset)*Twips) );
		 		}

//				(*PictString) += str;
		}
*/		// ������� �������� �� ��������� ����������� ����� �� ������
		//���������� ������ �����
		BOOL rc = TRUE;
		PInt8 pOutDIB = NULL;
		if(CPAGE_PictureGetPlace (h_Page, h_Pict,-pinfo.Incline2048, &Lr, &Wh) )
		{
			//piter : ������������ ���������� ��-�� ��������� ��������.
			switch(pinfo.Angle)
			{
			case 0:
				in.dwX = Lr.x;
				in.dwY = Lr.y;
				in.dwWidth = Wh.x;
				in.dwHeight = Wh.y;
				break;
			case 270:
				in.dwX = pinfo.Width - (Wh.y + Lr.y);
				in.dwY = Lr.x;
				in.dwWidth = Wh.y;
				in.dwHeight = Wh.x;
				break;
			case 180:
				in.dwX = pinfo.Width - (Wh.x + Lr.x);
				in.dwY = pinfo.Height - (Wh.y + Lr.y);
				in.dwWidth = Wh.x;
				in.dwHeight = Wh.y;
				break;
			case 90:
				in.dwX = Lr.y;
				in.dwY = pinfo.Height - (Wh.x + Lr.x);
				in.dwWidth = Wh.y;
				in.dwHeight = Wh.x;
				break;
			}
			// end piter
			LDPUMA_Skip(hTestDIBData);
			in.MaskFlag = FALSE;
			if(CIMAGE_GetDIBData((PWord8)PUMA_IMAGE_USER,&in,&pOutDIB))
			{// ������� �����������
				char szTurnName[]  ="RFRMT:TurnPicture";
				char szPictName[]  ="RFRMT:Picture";
				char szRotateName[]="RFRMT:RotatePicture";
				char * lpName = szPictName;


				LDPUMA_Skip(hTestTurn);
				if(CIMAGE_WriteDIB((PWord8)szPictName,pOutDIB,TRUE))
				{
					switch(pinfo.Angle)
					{
					case 90:
						rc = RIMAGE_Turn((PWord8)szPictName,(PWord8)szTurnName,RIMAGE_TURN_90,FALSE);
						CIMAGE_DeleteImage((PWord8)lpName);
						lpName = szTurnName;
						break;
					case 180:
						rc = RIMAGE_Turn((PWord8)szPictName,(PWord8)szTurnName,RIMAGE_TURN_180,FALSE);
						CIMAGE_DeleteImage((PWord8)lpName);
						lpName = szTurnName;
						break;
					case 270:
						rc = RIMAGE_Turn((PWord8)szPictName,(PWord8)szTurnName,RIMAGE_TURN_270,FALSE);
						CIMAGE_DeleteImage((PWord8)lpName);
						lpName = szTurnName;
						break;
					}
					if(!rc)
					{
						rc = FALSE;
					}
				}

				// �������� ����������� �� ����� ����.
				LDPUMA_Skip(hTestRotate);
				if(!RIMAGE_Rotate((PWord8)lpName,(PWord8)szRotateName,
					pinfo.Incline2048,2048, 0))
				{
					char * lp = (char*)RIMAGE_GetReturnString(RIMAGE_GetReturnCode());
					rc = FALSE;
				}
				else
				{
					CIMAGE_DeleteImage((PWord8)lpName);
					lpName = szRotateName;
				}

				// ��������� ���������� �����������
				Point32 ptLt,ptWh;
				if(rc && CPAGE_PictureGetPlace (h_Page, h_Pict,0, &ptLt, &ptWh) )
				{
					if(pinfo.Incline2048>=0)
					{
						in.dwX = ptWh.y*pinfo.Incline2048/2048;
						in.dwY = 0;
					}
					else
					{
						in.dwX = 0;
					 //  Beg of Almi Corr
 //						in.dwY = ptWh.x*pinfo.Incline2048/2048;
						in.dwY = (-ptWh.x*pinfo.Incline2048/2048);
						//  End of Almi Corr
					}
					if(!RIMAGE_RotatePoint((PWord8)lpName,
						in.dwX,in.dwY,
						(PInt32)&in.dwX,(PInt32)&in.dwY))
					{
						in.dwX = 0;
						in.dwY = 0;
					}

					in.dwWidth = ptWh.x;
					in.dwHeight = ptWh.y;
					in.wByteWidth = (unsigned short)((in.dwWidth +7)/8); //?
					in.MaskFlag = TRUE;
					// ������� ������ �����
					Word32 nSize = 0;
					LDPUMA_Skip(hTestGetMaskDIB);
					if(CPAGE_PictureGetMask (h_Page,h_Pict,0,NULL,&nSize))
					{
						char * lpMask = (char*)malloc(sizeof(in) + nSize );
						if(lpMask)
						{// �������� �����
							*(PCIMAGE_InfoDataInGet)lpMask = in;
							if(CPAGE_PictureGetMask (h_Page,h_Pict,0,lpMask + sizeof(in),&nSize))
							{
								if(!CIMAGE_GetDIBData((PWord8)lpName,(PCIMAGE_InfoDataInGet)lpMask,&pOutDIB))
								{
									rc = FALSE;
								}
							}
							else
							{
								rc = FALSE;
							}
							free(lpMask);
						}
					}
					else
					{
						rc = FALSE;
					}
				}
				LDPUMA_Skip(hTestWriteMetafile);
				if(rc)
				{
					if(RtfWriteMode)
						/*SaveMetafile(PictString,(BITMAPINFOHEADER*) pOutDIB)*/;
					else
					{
#ifdef EdWrite
  					LDPUMA_Skip(hTestWriteED);
					PCTDIB pTmpDIB = new CTDIB;
					pTmpDIB->SetDIBbyPtr(pOutDIB);

					pictSize.cx = int(Wh.x);
					pictSize.cy = int(Wh.y);
					pictGoal.cx = int((DWORD)(Twips* pTmpDIB->GetLineWidth()));
					pictGoal.cy = int((DWORD)(Twips* pTmpDIB->GetLinesNumber()));

					Int32 iDIBSize = pTmpDIB->GetDIBSize();
					delete pTmpDIB;

					indent.left    = 0;
					indent.right   = 0;
					indent.top     = 0;
					indent.bottom  = 0;
					interval.cx    = 0;
					interval.cy    = 0;
					playout.x   = -1;
					playout.w   = -1;
					playout.y   = -1;
					playout.h   = -1;

					Lr.x = MAX(0,Lr.x);
					Lr.y = MAX(0,Lr.y);

					slayout.left   = (int)(Lr.x);
					slayout.right  = (int)(Lr.x + Wh.x);
					slayout.top    = (int)(Lr.y);
					slayout.bottom = (int)(Lr.y + Wh.y);

					hPrevObject = SectorInfo->hObject;

					if(SectorInfo->FlagInColumn || (OutPutTypeFrame && SectorInfo->FlagFictiveParagraph))
					{
						hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hColumn, -1, indent,
						                                 SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
						hString = CED_CreateLine(hParagraph, 0,6);
						SectorInfo->FlagFictiveParagraph = FALSE;
					}

					if( FlagMode & USE_NONE || SectorInfo->CountFragments==1 )
						SectorInfo->hObject = SectorInfo->hColumn;
					else
					{
 						if(SectorInfo->FlagInColumn==TRUE)
						{
 							EdFragmRect.x = MAX(0,SectorInfo->OffsetFromColumn.x);
							EdFragmRect.y = MAX(0,SectorInfo->OffsetFromColumn.y);
 							EdFragmRect.w = (int)(MAX(0, Wh.x-FrameOffset)*Twips);
	 						EdFragmRect.h = (int)(Wh.y*Twips);
		 					SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn,
							                                      EdFragmRect, 0x22,-1, -1, -1);
						}
						else
						{
							EdFragmRect.x = (int)(Lr.x*Twips - SectorInfo->Offset.x);
							EdFragmRect.y = (int)(Lr.y*Twips - SectorInfo->Offset.y);
 							EdFragmRect.w = (int)(MAX(0, Wh.x-FrameOffset)*Twips);
	 						EdFragmRect.h = (int)(Wh.y*Twips);
		 					SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn,
			                                       			EdFragmRect, 0x22,-1, 0, 0);
						}
					}

					hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, -1, indent,
						SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
					hString = CED_CreateLine(hParagraph, 0,6);

					Letter.alternative = ' ';
					Letter.probability = 0;
					CED_CreateChar(hString, slayout, &Letter, 12,
						ED_PICT_BASE + (int)IndexPict, -1, -1, -1, -1);

					if( !CED_CreatePicture(SectorInfo->hEDPage, (int)IndexPict, pictSize,
						pictGoal, ED_ALIGN_MIDDLE, 1, pOutDIB, (int)iDIBSize) )
					{
     					SectorInfo->hObject = hPrevObject ;
						return FALSE;
					}
#endif
					LDPUMA_Skip(hTestDeleteImage);
					}
				}
				// piter
				// ����������� ������ ���������� �� pOutDIB
				CIMAGE_DeleteImage((PWord8)lpName);
				CIMAGE_FreeCopedDIB(pOutDIB);
				// end piter
			}
		}
/*		if(RtfWriteMode)
		{
			if(!(FlagMode & USE_NONE) && SectorInfo->CountFragments!=1 )
			{
 				(*PictString) += "\\f0\\fs6\\par}";
				if(SectorInfo->FlagInColumn==TRUE)
					(*PictString) +="}}}";
			}
		}
*/	}

#ifdef EdWrite
	if(!RtfWriteMode)
 	SectorInfo->hObject = hPrevObject ;
#endif
	LDPUMA_Skip(hTestEnd);

	return TRUE;
}


// Piter.
// ���������� ����������� � ���������
//
//
static void bufcpy(char ** str, void  * mem, unsigned sz)
{
	const char Hex[]="0123456789ABCDEF";
	unsigned char * c = (unsigned char *)mem;
	for(unsigned i=0; i<sz; i++,(*str)+=2)
	{
		(*str)[0] = Hex[c[i]>>4];
		(*str)[1] = Hex[c[i]&0x0F];
		(*str)[2] = 0;
	}
}
/*
BOOL SaveMetafile(CString * strBuf, BITMAPINFOHEADER * lpDIB)
{
	BOOL rc = FALSE;

#pragma pack (push,1)
	struct MF_header {
		WORD 	mtType;
		WORD 	mtHeaderSize; // in words
		WORD 	mtVersion;
		DWORD	mtSize;       // in words
		WORD  	mtNoObjects;
		DWORD 	mtMaxRecord;
		WORD	mtNoParameters;
	} hMF={1,9,0x0300,0,0,0,0};

	struct MF_GDI_records {
		DWORD   rdSize;
		WORD    rdFunction;
	} MF_GDI;

	struct MF_StretchDlBitst_info {
		DWORD	dwRop;
		WORD	wUsage;
		WORD	srcYExt;
		WORD	srcXExt;
		WORD 	srcY;
		WORD	srcX;
		WORD	dstYExt;
		WORD	dstXExt;
		WORD	dstY;
		WORD	dstX;
	}  MF_SI;
#pragma pack (pop)

	// ��������� ��������
	CString text;

	int nPalette = 0;

	if(lpDIB->biBitCount == 1)
		nPalette = 2;
	else if(lpDIB->biBitCount == 4)
		nPalette = 16;
	else if(lpDIB->biBitCount == 8)
		nPalette = 256;

	int SizePicture = sizeof(BITMAPINFOHEADER)
		+ sizeof(RGBQUAD) * nPalette
		+ lpDIB->biSizeImage;

	hMF.mtMaxRecord=
		(sizeof(MF_GDI) + sizeof(MF_SI) +
		+ SizePicture)/2L;

	// 3. ������ ���䠩�� � ᫮���
	hMF.mtSize= sizeof(hMF)/2 +       // header
		sizeof(MF_GDI)/2 + 2 +// SetWindowOrg
		sizeof(MF_GDI)/2 + 2 +// SetWindowExt
		sizeof(MF_GDI)/2 + 2 +// SetTextColor
		sizeof(MF_GDI)/2 + 2 +// SetBkColor
		sizeof(MF_GDI)/2 + 1 +// StretchBltMode
		hMF.mtMaxRecord +
		sizeof(MF_GDI)/2;     // End
	//
	// �������� ����� ��������
	//
		text.Format("{\\pict\\wmetafile8\\picw%li\\pich%li"
			"\\picwgoal%li\\pichgoal%li\\sspicalign0 ",
			(DWORD)(175L*Twips/100*lpDIB->biWidth),
			(DWORD)(175L*Twips/100*lpDIB->biHeight),
			(DWORD)(Twips* lpDIB->biWidth),
			(DWORD)(Twips* lpDIB->biHeight)
			);

		*strBuf = *strBuf + text;
	long OldSizestrPict = strBuf->GetLength();
	long NewSizestrPict = OldSizestrPict + hMF.mtSize * 4 + 1 + 1;
	char * strPic = strBuf->GetBuffer(NewSizestrPict);
	if(strPic)
	{
		char * str = strPic + OldSizestrPict;

		// header
		bufcpy(&str,&hMF,sizeof(hMF));

		// SetWindowOrg
		MF_GDI.rdSize=5;
		MF_GDI.rdFunction=0x20b;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));
		DWORD dword=0L;
		bufcpy(&str,&dword,sizeof(dword));
		// SetWindowExt
		MF_GDI.rdSize=5;
		MF_GDI.rdFunction=0x20c;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));
		WORD word=(WORD)lpDIB->biHeight;
		bufcpy(&str,&word,sizeof(word));
		word=(WORD)lpDIB->biWidth;
		bufcpy(&str,&word,sizeof(word));
		// SetTextColor
		MF_GDI.rdSize=5;
		MF_GDI.rdFunction=0x209;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));
		dword=0;
		bufcpy(&str,&dword,sizeof(dword));
		// SetBkColor
		MF_GDI.rdSize=5;
		MF_GDI.rdFunction=0x201;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));
		dword=0x00008080;
		bufcpy(&str,&dword,sizeof(dword));
		// StretchBltMode
		MF_GDI.rdSize=4;
		MF_GDI.rdFunction=0x107;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));
		word=1;
		bufcpy(&str,&word,sizeof(word));
		// Picture BMP
		MF_GDI.rdSize=hMF.mtMaxRecord;
		MF_GDI.rdFunction=0xf43;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));

		MF_SI.dwRop	=0x00cc0020L;
		MF_SI.wUsage	=0;
		MF_SI.srcYExt	=(WORD)lpDIB->biHeight;
		MF_SI.srcXExt	=(WORD)lpDIB->biWidth ;
		MF_SI.srcY	=0;
		MF_SI.srcX	=0;
		MF_SI.dstYExt	=(WORD)lpDIB->biHeight;
		MF_SI.dstXExt	=(WORD)lpDIB->biWidth ;
		MF_SI.dstY	=0;
		MF_SI.dstX	=0;
		bufcpy(&str,&MF_SI,sizeof(MF_SI));

		bufcpy(&str,lpDIB,SizePicture);
		// End
		MF_GDI.rdSize=3;
		MF_GDI.rdFunction=0;
		bufcpy(&str,&MF_GDI,sizeof(MF_GDI));
//  *(str++)='}';
  *(++str)='}';    //ALIK 22012000
	 strBuf->ReleaseBuffer(NewSizestrPict);
	}
	else
	{
		*strBuf += "}";
		rc = FALSE;
	}

	return rc;
}
*/






