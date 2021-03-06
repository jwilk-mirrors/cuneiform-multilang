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


// Pictures.cpp

//********************************************************************
//********************************************************************

#include <string.h>
#include "stdafx.h"
#include "rout_own.h"

#include "compat_defs.h"

//********************************************************************
BOOL PictureFromChar(Handle charHandle)
{
/* �������� �������� �������� �� �����������
	����������:
	gPictureNumber - ����� �������� �� 1
	gPictureData   - ����� DIB ������� ���������
	gPictureLength - ����� DIB ������� ���������

*/
	gPictureData = 0;
	gPictureLength = 0;
	memset (&gPictureSize,0,sizeof(gPictureSize));
	memset (&gPictureGoal,0,sizeof(gPictureGoal));

	gPictureNumber = GetPictureNumber(charHandle);

	long pictUserNumber=0, pictAlign=0, type=0;
	long picturesCount = CED_GetNumOfPics(gPageHandle);

	for ( long index = 0; index < picturesCount; index++ )
		{
		if( CED_GetPicture(
			gPageHandle,
			index,
			(int*)&pictUserNumber, // ���������������� �����
			&gPictureSize, // ������ �������� � TIFF-����� � ��������
			&gPictureGoal, // ������ �������� �� ������ � twips
			(int*)&pictAlign,	   // ������������ ������������
			(int*)&type,		   // ��� = 1 (DIB)
			(void**)&gPictureData, // ����� DIB ������� ���������
			(int*)&gPictureLength  // ����� DIB ������� ���������
			) &&
			pictUserNumber == gPictureNumber
		   )
			{
			if (!gPictureData ||
				gPictureLength < 0
				)
				{
				ERR_PICTURE_DATA;
				MyDebugPrint("pictUserNumber = %d",pictUserNumber);
				goto Fail;
				}
			return TRUE;
			}
		}
//	ASSERT(gPictureData);
//	ASSERT(gPictureLength >= 0);
Fail:
	gPictureData = 0;
	gPictureLength = 0;
	memset (&gPictureSize,0,sizeof(gPictureSize));
	memset (&gPictureGoal,0,sizeof(gPictureGoal));
	gPictureNumber = -1;

	return FALSE;
}
//********************************************************************
long GetPictureNumber(Handle charHandle)
{
// ������� ����� �������� �� �����������
	long fontNum = 0, pictureNumber = 0;

	fontNum = CED_GetCharFontNum(charHandle);
	ASSERT (fontNum >= ED_PICT_BASE);
	pictureNumber = fontNum - ED_PICT_BASE;

	return pictureNumber;
}
//********************************************************************
BOOL WritePictureToBMP_File(
					Byte *pDIB,	   // ����� DIB ������� ���������
					long lenDIB,   // ����� DIB ������� ���������
					char *filename // ��� �����
					)
{
/* store dib as bmp file
 bmp file format description could be found at 
 http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
*/
    	BITMAPFILEHEADER   bf = {0}; //  bmp fileheader
	BITMAPINFOHEADER  * bfinfo;
	// crete output file
	FILE* f = fopen(filename, "wb");

	if (!f)
		{
		ERR_OPEN_FILE;
		return FALSE;
		}
	
	// BMP file header
	bfinfo =  (BITMAPINFOHEADER *)pDIB;
	bf.bfType = 0x4d42; // 'BM'

	bf.bfSize = sizeof(BITMAPFILEHEADER) + lenDIB;
	// fileheader + infoheader + palette
	bf.bfOffBits = sizeof(BITMAPFILEHEADER) +
					sizeof(BITMAPINFOHEADER)+ bfinfo->biClrUsed* sizeof(RGBQUAD);

	if(fwrite((char*)&bf,1,sizeof(bf), f) != sizeof(bf))
		{
		fclose(f);
		ERR_WRITING_TO_FILE;
		return FALSE;
		}

	// write dib, including file header
	if(fwrite((char*)pDIB, 1, lenDIB, f) != (size_t)lenDIB)
		{
		fclose(f);
		ERR_WRITING_TO_FILE;
		return FALSE;
		}

	if(fclose(f))
		{
		ERR_CLOSE_FILE;
		return FALSE;
		}

	return TRUE;
}
//********************************************************************
