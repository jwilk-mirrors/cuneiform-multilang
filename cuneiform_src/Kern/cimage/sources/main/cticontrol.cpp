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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "cticontrol.h"
#include "ctimemory.h"
#include "dpuma.h"

/////////////////////////////////////////////////
//#define CIMAGE_DUMP_ENABLE

#ifdef CIMAGE_DUMP_ENABLE
#include "PumaMemoryToFileDumper.h"
#endif
//////////////////////////////////////////////////
//#define _TRASING
//
#ifdef _TRASING
#include "afx.h"
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTIControl::CTIControl()
{
	mbSourceDIBCopy = FALSE;
	hCBImage      = NULL;
	pCBImage      = NULL;
	pCBBuffer     = NULL;
	wCBBufferSize = 0;
	wCBLine       = 0;
	wCBLines      = 0;
	wCBWidth      = 0;
	wCBStep       = 0;
	mCBSourceDIB        = NULL;
	mCBDestianationDIB  = NULL;
	mCBWSourceDIB       = NULL;
	mCBWDestianationDIB = NULL;
	mCBWInProcess       = FALSE;
	mCBWName[0]         = 0x0;
	mCBName[0]          = 0x0;
	mpDIBFromImage      = NULL;
	mpBitFildFromImage  = NULL;
	mhBitFildFromImage  = NULL;
	mpcSrcDIBReadMask   = NULL;
	mpcSrcDIBWriteMask  = NULL;
	mbEnableDIBReadMask = TRUE;
	mbEnableDIBWriteMask= TRUE;
	mwMemoryErrors      = 0;
	mbWriteFlag         = FALSE;
	mhCopyedDIB         = NULL;

	mwLAWhiteLeftMask[7]  = 0xff;
	mwLAWhiteRightMask[0] = 0xff;
	mwLABlackLeftMask[0]  = 0x7f;
	mwLABlackRightMask[7] = 0xfe;
	mwIndexMask[0]        = 0x01;
	for ( Word32 i = 1; i < 8 ; i++ )
	{
		mwLAWhiteLeftMask[7 - i]  = mwLAWhiteLeftMask[8 - i] << 1;
		mwLAWhiteRightMask[i]     = mwLAWhiteRightMask[i - 1] >> 1;
		mwLABlackLeftMask[i]      = mwLABlackLeftMask[i - 1] >> 1;
		mwLABlackRightMask[7 - i] = mwLABlackRightMask[8 - i] << 1;
		mwIndexMask[i]            = mwIndexMask[i - 1] << 1;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTIControl::~CTIControl()
{
	FreeBuffers();

	if ( mCBSourceDIB != NULL )
	{
		delete mCBSourceDIB;
	}
	if ( mCBWSourceDIB != NULL )
	{
		delete mCBWSourceDIB;
	}
	if ( mCBDestianationDIB != NULL )
	{
		delete mCBDestianationDIB;
	}
	if ( mCBWDestianationDIB != NULL )
	{
		delete mCBWDestianationDIB;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::WriteCBImage(PChar8  lpName, CIMAGEIMAGECALLBACK Cbk )
{
	Handle hNewDIB;
	Bool32 Ret;
	Word32 Readed;
	Word32 i;
	Bool32 bInvert = false;
	CIMAGE_InfoDataInReplace   FrameToReplace;
	CIMAGE_ImageInfo           ImageInfo;
	CTDIBRGBQUAD               cdFQuad;
	CTDIBRGBQUAD               cdSQuad;

	//////////////////////////////////////////////////////////////////////
	//�������� ����� ��������
	if ( !lpName || lpName[0] == 0x0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGE_NAME);
		return FALSE;
	}
	//
	ImageInfo.wImageWidth = 0;
	ImageInfo.wImageByteWidth = 0;
	ImageInfo.wImageHeight = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ������� ����� DIB
	if ( mCBWDestianationDIB != NULL || mCBWSourceDIB != NULL )
	{
		return FALSE;
	}
	// ��� �������� �� ��������� ����� ����
	mCBWInProcess = TRUE;
	if ( strlen( lpName ) < CIMAGE_MAX_IMAGE_NAME)
	{
		strcpy(mCBWName, lpName);
	}
	else
	{
		strncpy(mCBWName, lpName, CIMAGE_MAX_IMAGE_NAME - 1);
	}
	// ���� � �������� ������ ��� ����������� �� ����� ������
	mCBWSourceDIB       = new CTDIB;
	// � ���� ����� ��������
	mCBWDestianationDIB = new CTDIB;
	///////////////////////////////////////////////////////////////////////////////////
	// ���������� ���������� � �����������
	Ret = Cbk.CIMAGE_ImageOpen(&ImageInfo);

	if ( Ret == FALSE )
	{
		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK);
		mCBWInProcess = FALSE;
		return FALSE;
	}

	if ( ImageInfo.wImageByteWidth == 0 || ImageInfo.wImageHeight == 0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK);
		mCBWInProcess = FALSE;
		return FALSE;
	}
	// ��������� ������ ��� ����� DIB � ��� ��������
	if ( mCBWDestianationDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) &&
		 mCBWSourceDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock)    )
	{
		CIMAGEComment(lpName);
		if ( mCBWDestianationDIB->CreateDIBBegin(ImageInfo.wImageWidth, ImageInfo.wImageHeight, 1) &&
			 mCBWDestianationDIB->SetResolutionDPI(ImageInfo.wResolutionX, ImageInfo.wResolutionY) &&
			 mCBWDestianationDIB->CreateDIBEnd() )
		{
			CIMAGEComment("Temporary DIB - WriteCBImage");
			if ( mCBWSourceDIB->CreateDIBBegin(ImageInfo.wImageWidth, 1, 1) &&
				 mCBWSourceDIB->CreateDIBEnd() )
			{
				cdFQuad.rgbBlue = cdFQuad.rgbGreen = cdFQuad.rgbRed = 0x00;
				cdSQuad.rgbBlue = cdSQuad.rgbGreen = cdSQuad.rgbRed = 0xff;

				if (ImageInfo.bFotoMetrics == 0)
				{
					bInvert = true;
				}

				mCBWDestianationDIB->SetRGBQuad(0,cdFQuad);
				mCBWSourceDIB->SetRGBQuad(0,cdFQuad);
				mCBWDestianationDIB->SetRGBQuad(1,cdSQuad);
				mCBWSourceDIB->SetRGBQuad(1,cdSQuad);
				// ��������� ����������	��� ��������� �����
				FrameToReplace.byBit      = (Word16)mCBWSourceDIB->GetPixelSize();
				FrameToReplace.dwX        = 0;
				FrameToReplace.dwY        = 0;
				FrameToReplace.dwWidth    = (Word16)mCBWSourceDIB->GetLineWidth();
				FrameToReplace.dwHeight   = (Word16)mCBWSourceDIB->GetLinesNumber();
				FrameToReplace.wByteWidth = (Word16)mCBWSourceDIB->GetLineWidthInBytes();
				FrameToReplace.lpData     = (PWord8)mCBWSourceDIB->GetPtrToLine(0);
				FrameToReplace.MaskFlag   = 0;

				// ��������� ���
				for ( i = 0; i < ImageInfo.wImageHeight; i++ )
				{
					// �������� ������ ������
					Readed = Cbk.CIMAGE_ImageRead((PInt8)mCBWSourceDIB->GetPtrToLine(0),
												  (Word16)mCBWSourceDIB->GetLineWidthInBytes());

					//����������� ������� ����, ����� ����
					if ( bInvert )
					{
						PInt8 pBits = (PInt8)mCBWSourceDIB->GetPtrToLine(0);
						Int32 Ii;

						for ( Ii = 0; Ii < (Word16)mCBWSourceDIB->GetLineWidthInBytes(); Ii++ )
						{
							*pBits++ = ~(*pBits);
						}
					}

					// ����� �������
					FrameToReplace.dwY = i;
					// ��������� ������� � ����� �� ������
					if (Readed)
					{
						SetFrame(mCBWSourceDIB, mCBWDestianationDIB, &FrameToReplace);
					}
				}
				// ���������
				// ��������� ������ ������
				Ret = Cbk.CIMAGE_ImageClose();
				// ���������� � ���������
				mCBWDestianationDIB->GetDIBHandle(&hNewDIB);
				// ����� ��������, � �� ����� �����!!!
				mbWriteFlag = TRUE;
				Ret = SetDIB(lpName, hNewDIB, 0);
				mbWriteFlag = FALSE;

				if ( Ret == FALSE )
				{
					mCBWInProcess = FALSE;
					return FALSE;
				}
			}
		}
	}


	if ( mCBWSourceDIB )
	{
		mCBWSourceDIB->DestroyDIB();
		delete mCBWSourceDIB;
		mCBWSourceDIB = NULL;
	}

#ifdef CIMAGE_DUMP_ENABLE

	//WriteDIBtoBMP("Allex.CBWrited.bmp", mCBWDestianationDIB);

#endif

	if ( mCBWDestianationDIB )
	{
		mCBWDestianationDIB->DestroyDIB();
		delete mCBWDestianationDIB;
		mCBWDestianationDIB = NULL;
	}

	mCBWInProcess = FALSE;
	return Ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetCBImage (PChar8  lpName, PCIMAGEIMAGECALLBACK pCbk)
{
	Handle  hImage = NULL;

	if ( !pCbk  )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( !GetDIB(lpName, &hImage, TRUE ) )
	{
		return FALSE;
	}

#ifdef CIMAGE_DUMP_ENABLE
	PCTDIB  pDebugDIB;

	pDebugDIB = new CTDIB();
	pDebugDIB->SetDIBbyPtr(hImage);
	//WriteDIBtoBMP("Allex.CBReaded.bmp",pDebugDIB);
	delete pDebugDIB;
#endif

	if ( strlen( lpName ) < CIMAGE_MAX_IMAGE_NAME)
	{
		strcpy(mCBName, lpName);
	}
	else
	{
		strncpy(mCBName, lpName, CIMAGE_MAX_IMAGE_NAME - 1);
	}
	/////////////////////////////////////////
	pCbk->CIMAGE_ImageOpen  = CIMAGE_Callback_ImageOpen;
	pCbk->CIMAGE_ImageRead  = CIMAGE_Callback_ImageRead;
	pCbk->CIMAGE_ImageClose = CIMAGE_Callback_ImageClose;
	wCBStep = 0;

	hCBImage = hImage;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::SetDIB(PChar8  lpName, Handle hDIB, Word32 wFlag)
{
	Handle  hImage = NULL;

	if ( wFlag == 0 )
	{  // ������� ����� �����
		if ( !CopyDIB( hDIB, &hImage) )
		{
			SetReturnCode_cimage(IDS_CIMAGE_UNABLE_COPY_DIB);
			return FALSE;
		}
	}
	else
	{
		hImage = hDIB;
	}

	return mlImages.AddImage(lpName, hImage, wFlag);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetDIB(PChar8  lpName, PHandle phDIB, Word32 wFlag)
{
	Handle  hImage = NULL;  // Handle

	// ��������� ��������
	if ( !OpenDIBFromList( lpName, &hImage ) )
		return FALSE;

	//ALLEX Mask
	// ������ �� ���
	// ��������� ����� (���� ����)
	if ( !OpenMaskFromList( lpName, &mpcSrcDIBReadMask, &mbEnableDIBReadMask, "r" ) ||
		 !OpenMaskFromList( lpName, &mpcSrcDIBWriteMask, &mbEnableDIBWriteMask, "w" )  )
		 return FALSE;


	mbSourceDIBCopy = (wFlag == 0 );
	if ( mbSourceDIBCopy )
	{  // ������� ����� �����
		if ( CopyDIB( hImage, phDIB) )
		{
			return TRUE;
		}

		SetReturnCode_cimage(IDS_CIMAGE_UNABLE_COPY_DIB);
	}
	else
	{
		*phDIB = hImage;
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetImage(PChar8  lpName, PCIMAGE_InfoDataInGet lpIn, PCIMAGE_InfoDataOutGet lplpOut)
{
	Bool32   bRet = FALSE;
	PCTDIB   pDscDIB;
	void*    pDIBMemory;
	Int32    iY;

	FreeBuffers();

	// ����� ����� ���� ��������
	if ( GetDIBFromImage(lpName, lpIn, (PInt8*)&pDIBMemory ) )
	{
		pDscDIB = new CTDIB;
		if ( pDscDIB->SetDIBbyPtr(pDIBMemory) )
		{
#ifdef CIMAGE_DUMP_ENABLE
			//WriteDIBtoBMP("Allex.AlmiData.bmp",pDscDIB);
#endif
			//////////////////////////////////////
			// �������, ��� ��� �� �����;
			if ( lpIn->wByteWidth >= pDscDIB->GetUsedLineWidthInBytes()
				 && lpIn->dwWidth    == pDscDIB->GetLineWidth()
				 && lpIn->dwHeight   == pDscDIB->GetLinesNumber()
				)
			{
				Word32 nOutLine;
				PWord8 pOutLine;
				Word8  WhiteBit;

				lplpOut->byBit      = (Word16)pDscDIB->GetPixelSize();
				lplpOut->dwHeight   = pDscDIB->GetLinesNumber();
				lplpOut->dwWidth    = pDscDIB->GetLineWidth();
				lplpOut->wByteWidth = (Word16)pDscDIB->GetUsedLineWidthInBytes();

				lplpOut->wBlackBit  = pDscDIB->GetBlackPixel();

				WhiteBit = (Word8)pDscDIB->GetWhitePixel();

				mhBitFildFromImage = CIMAGEDAlloc(lpIn->wByteWidth * lpIn->dwHeight,lpName);
				mpBitFildFromImage = (PWord8)CIMAGELock(mhBitFildFromImage);

				if ( !mhBitFildFromImage || !mpBitFildFromImage )
				{
					if ( mhBitFildFromImage )
						CIMAGEFree(mhBitFildFromImage);

					delete pDscDIB;
					SetReturnCode_cimage(IDS_CIMAGE_ERR_NO_MEMORY);
					return FALSE;
				}

				lplpOut->lpData = pOutLine = mpBitFildFromImage;
				iY = (Int32)lpIn->dwY;

			    // ��� Almi - ��������� ����� ��������
				/*
				for ( ;iY < 0; iY++, lplpOut->dwHeight++ )
				{
					// �������� ���������� � lplpOut.lpData
					CIMAGE_MEMSET(pOutLine, WhiteBit, lplpOut->wByteWidth);
					pOutLine += lplpOut->wByteWidth;
				}
				*/
				//end ��� Almi

				for ( nOutLine = 0;nOutLine < lplpOut->dwHeight; nOutLine++ )
				{
					// �������� ���������� � lplpOut.lpData
					CIMAGE_MEMCOPY(pOutLine, pDscDIB->GetPtrToLine(nOutLine), lplpOut->wByteWidth);
					pOutLine += lplpOut->wByteWidth;
				}

				bRet = TRUE;
			}
		}

		delete pDscDIB;
	}
	else
	{
		bRet = FALSE;
		lplpOut->lpData = NULL;
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::ReplaceImage(PChar8  lpName, PCIMAGE_InfoDataInReplace lpIn)
{
	Handle         hImage = NULL;
	void *         pImage;
	PCTDIB         pSrcDIB = NULL;
	PCTDIB         pDscDIB = NULL;
	Bool32         ret = TRUE;

	if ( !GetDIB(lpName, &hImage, TRUE ) )
	{
		return FALSE;

	}

	pImage = CIMAGELock(hImage);

	if ( !pImage )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		ret =  FALSE;
	}

	//�������������� ctdib-����
	pSrcDIB = new CTDIB;
	pDscDIB = new CTDIB;

	if ( pDscDIB->SetDIBbyPtr(pImage) )
	{
		if ( pSrcDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) )
		{
			CIMAGEComment("Temporary DIB - ReplaceImage");
			if ( pSrcDIB->CreateDIBBegin(lpIn->dwWidth,lpIn->dwHeight,pDscDIB->GetPixelSize(),pDscDIB->GetActualColorNumber()) &&
				 pSrcDIB->CreateDIBEnd() )
			{
				if ( lpIn->wByteWidth <= pSrcDIB->GetUsedLineWidthInBytes() )
				{
					Word32 nInLine;
					PWord8 pInLine = lpIn->lpData;

					for ( nInLine = 0; nInLine < lpIn->dwHeight; nInLine++ )
					{
						// �������� ���������� � lplpOut.lpData
						CIMAGE_MEMCOPY( pSrcDIB->GetPtrToLine(nInLine), pInLine, lpIn->wByteWidth);
						pInLine += lpIn->wByteWidth;
					}

					if ( !SetFrame(pSrcDIB, pDscDIB, lpIn) )
					{
						SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
						ret = FALSE;
					}
				}
			}
		}
	}

	CIMAGEUnlock(hImage);
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetImageInfo(PChar8  lpName, PCIMAGEBITMAPINFOHEADER lpBIH)
{
	Handle hImage = NULL;
	void * pDIB;

	if ( !lpBIH )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( !GetDIB(lpName, &hImage, TRUE) )
	{
		return FALSE;
	}

	pDIB = CIMAGELock(hImage);

	CIMAGE_MEMCOPY(lpBIH, pDIB, sizeof(CIMAGEBITMAPINFOHEADER));

	CIMAGEUnlock(hImage);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::RemoveImage(PChar8  lpName)
{
	return mlImages.DeleteImage(lpName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CopyDIB(Handle hSrc, PHandle phCopyedDib)
{
	void *         pSrc = NULL;
	void *         pDsc = NULL;
	Handle         hDsc = NULL;
	Word32         DIBSize = 0;
	PCTDIB         pcSrcDIB = NULL;
	PCTDIB         pcDscDIB = NULL;

	pSrc = CIMAGELock(hSrc);

	if ( !pSrc )
	{
		return FALSE;
	}

	pcSrcDIB   = new CTDIB;

	if ( !pcSrcDIB->SetDIBbyPtr(pSrc) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_DIB);
		delete pcSrcDIB;
		return FALSE;
	}

	DIBSize = pcSrcDIB->GetDIBSize();

	// ������ �� �����
	delete pcSrcDIB;

	hDsc = CIMAGEDAlloc(DIBSize, "Copyed DIB - CopyDIB");
	pDsc = CIMAGELock(hDsc);

	if ( !hDsc || !pDsc )
	{
		return FALSE;
	}

	CIMAGE_MEMCOPY(pDsc, pSrc, DIBSize);

	//ALLEX Mask/////////////////////////////////////
	// ������ ���
	pcDscDIB   = new CTDIB;

	if ( !pcDscDIB->SetDIBbyPtr(pDsc) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_DIB);
		delete pcDscDIB;
		return FALSE;
	}
	// � ���� ������ ���� �������� ����� �� ������ ��� ��������
	if ( mpcSrcDIBReadMask && mbEnableDIBReadMask && mbWriteFlag == FALSE )
	{
		ApplayMaskToDIB(pcDscDIB, mpcSrcDIBReadMask);
	}
	delete pcDscDIB;
	//////////////////////////////////////////////////
	CIMAGEUnlock(hDsc);
	CIMAGEUnlock(hSrc);

	*phCopyedDib = mhCopyedDIB = hDsc;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::GetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInGet pIn, PWord8 pMask)
{
	if ( pSrcDIB == NULL || pDscDIB == NULL )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	if ( ( pSrcDIB->GetLineWidth() < pDscDIB->GetLineWidth() + pIn->dwX ) ||
		 ( pSrcDIB->GetLinesNumber() < pDscDIB->GetLinesNumber() + pIn->dwY ) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}
	//�������� �� ������ ���� � ������
	if ( !CopyToFrame(pSrcDIB, pDscDIB, pIn, pMask) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	//ALLEX Mask
	// ������ �� ���
	//������ �������� ����� � �����
	if ( mpcSrcDIBReadMask && mbEnableDIBReadMask)
	{
		if ( !ApplayMaskToDIB(pDscDIB, mpcSrcDIBReadMask, pIn->dwX, pIn->dwY) )
			return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CopyToFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInGet pFrameInfo, PWord8 pMask)
{
	Word32 StartY = pFrameInfo->dwY;
	Word32 EndY   = StartY + pFrameInfo->dwHeight;
	Word32 StartX = pFrameInfo->dwX;
	Word32 nLine;
	Word32 dLine = 0;
	Bool32 bRet = FALSE;

	if ( pSrcDIB->GetPixelSize() != pSrcDIB->GetPixelSize() )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	for ( nLine = StartY; nLine < EndY; nLine++)
	{
		if ( !pDscDIB->SetFuelLineFromDIB(pSrcDIB, nLine, dLine++, StartX) )
		{
			SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
			return FALSE;
		}
		bRet = TRUE;
	}
	// ������ ���������� ������� �����
	WriteDIBtoBMP("Allex.DIBCopyToFrame.Before Bit Mask.bmp", pDscDIB);

	if( pMask != NULL &&
		pDscDIB->GetLinesNumber() == pFrameInfo->dwHeight &&
		pDscDIB->GetLineWidth()   == pFrameInfo->dwWidth     )
	{
		bRet = ApplayBitMaskToDIB( pMask, pDscDIB );
		WriteDIBtoBMP("Allex.DIBCopyToFrame.After Bit Mask.bmp", pDscDIB);
	}
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CopyFromFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInReplace pFrameIn)
{
	Word32 StartY = pFrameIn->dwY;
	Word32 EndY   = StartY + pFrameIn->dwHeight;
	Word32 nLine;
	Word32 dLine = 0;

	if ( pSrcDIB->GetPixelSize() != pSrcDIB->GetPixelSize() )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	for ( nLine = StartY; nLine < EndY; nLine++)
	{
		// ���� ������ ���������, ��
		if ( pDscDIB->GetLineWidth() == pSrcDIB->GetLineWidth() )
		{
			if ( !pDscDIB->SetFuelLineFromDIB(pSrcDIB, dLine++, nLine, 0) )
			{
				SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
				return FALSE;
			}
		}
		else
		{
			continue;
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::ApplayBitMaskToFrame(PCIMAGE_InfoDataInGet pIn, PCIMAGE_InfoDataOutGet pOut)
{

	if ( pIn->MaskFlag == 0 )
	{
		return TRUE;
	}
	else
	{
		Word32 SizeInBytes;
		Word32 i;
		char * pMask = (char *)pIn;
		char * pRaster = (char *)(pOut->lpData);
		char * Msc;
		char * Dsc;

		pMask += sizeof(CIMAGE_InfoDataInGet);

		switch (pOut->byBit)
		{

		case 1:

			SizeInBytes = (pOut->dwWidth) * (pOut->dwHeight);
			SizeInBytes += 7;
			SizeInBytes /= 8;

			Dsc = pRaster;
			Msc = pMask;

			for ( i = 0; i < (SizeInBytes - 1); i++ )
			{
				 *Dsc = (*(Dsc++) & *(Msc++));
			}

			break;

		default:
			SetReturnCode_cimage(IDS_CIMAGE_NO_SUPPORT_DIB_FORMAT_FOR_THIS_ACTION);
			return FALSE;

		}

	}

	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::ApplayBitMaskToDIB(PWord8 pMask, PCTDIB pDIB)
{
	if ( pMask == 0 )
	{
		return TRUE;
	}
	else
	{
		PWord8 pMaskByte = pMask; //(PWord8)&pIn[1];
		PWord8 pImageByte;
		Word32 Lines = pDIB->GetLinesNumber();
		Word32 Pixels = pDIB->GetLineWidth();
		Int32 i;
		Int32 j;
		Int32 k;
		Word8  WhiteByte = (Word8)pDIB->GetWhitePixel();
		Word8  tmpByte;

		///////////////////////////////////////////////////////////////////////////////////////
		//Allex 28.01.2000
		//����� ������������� ������, ��������� �  ���������� ������ ��� ������������ ��� �����
		//����� ��������� ��������:
		Word8 TestByte;
		Word32 MaskLenght = ((Pixels + 7)/8)*Lines;
		PWord8 pMaskBegin = pMask;
		PWord8 pMaskEnd = pMaskBegin + MaskLenght - 1;

		try
		{
			TestByte = *pMaskBegin;
			*pMaskBegin = TestByte;
			TestByte = *pMaskEnd;
			*pMaskEnd = TestByte;
		}

		catch(...)
		{
			SetReturnCode_cimage(IDS_CIMAGE_INVALID_MASK_MEMORY);
			LDPUMA_Console("CImage: Mask unvailable or mask memory blok less then needed!\n        Mask not applied.\n");
			return TRUE;
		}
		// ����� ��������
		///////////////////////////////////////////////////////////////////////////////////////

		for ( i = 0; i < (Int32)Lines; i++ )
		{
			pImageByte = (PWord8)pDIB->GetPtrToLine( i );
			j = Pixels;
			//��������������� �� ������ � �����
			pMaskByte = pMask + ((Pixels + 7)/8)*i;

			switch ( pDIB->GetPixelSize() )
			{
			case 1:
				while ( (j > 0) )
				{
					tmpByte = ~(*(pMaskByte));

					if ( WhiteByte == 1 )
					{
						*(pImageByte++) = *(pImageByte) | tmpByte;
					}
					else
					{
						*(pImageByte++) = *(pImageByte) & tmpByte;
					}

					pMaskByte++;
					j -= 8;
				}
				break;

			case 8:
				while ( j > 7 )
				{
					tmpByte = ~(*(pMaskByte));

					for ( k = 7; k >= 0; k--)
					{
						if ( tmpByte & mwIndexMask[k] )
						{
							*(pImageByte) = WhiteByte;
						}

						pImageByte++;
					}

					pMaskByte++;
					j -= 8;
				}

				k = 7;
				while ( j-- )
				{
					tmpByte = ~(*(pMaskByte));

					if ( tmpByte & mwIndexMask[k--] )
					{
						*(pImageByte) = WhiteByte;
					}
				}
					pMaskByte++;
				break;

			case 24:
				while ( j > 7 )
				{
					tmpByte = ~(*(pMaskByte));

					for ( k = 7; k >= 0; k--)
					{
						if ( tmpByte & mwIndexMask[k] )
						{
							*(pImageByte)     = WhiteByte;
							*(pImageByte + 1) = WhiteByte;
							*(pImageByte + 2) = WhiteByte;
						}

						pImageByte += 3;
					}

					pMaskByte++;
					j -= 8;

				}

				k = 7;
				while ( j-- )
				{
					tmpByte = ~(*(pMaskByte));

					if ( tmpByte & mwIndexMask[k--] )
					{
							*(pImageByte) = WhiteByte;
							*(pImageByte + 1) = WhiteByte;
							*(pImageByte + 2) = WhiteByte;
					}
				}
				break;

			default:
				SetReturnCode_cimage(IDS_CIMAGE_NOT_REALIZED);
				return FALSE;
			}
		}

		return TRUE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::SetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInReplace pIn)
{
	Word32 wFrameSize;

	if ( pSrcDIB == NULL || pDscDIB == NULL || !pIn)
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}
	// ��������� �� �����������
	wFrameSize = (pIn->dwHeight) * (pIn->dwWidth);

	if ( wFrameSize == 0 )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
		return FALSE;
	}
	// ��������� �� ������������ ��������
	if ( ( pDscDIB->GetLineWidth()   < pSrcDIB->GetLineWidth() + pIn->dwX ) ||
		 ( pDscDIB->GetLinesNumber() < pSrcDIB->GetLinesNumber() + pIn->dwY ) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
		return FALSE;
	}

	if ( !CopyFromFrame(pSrcDIB, pDscDIB, pIn) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CBImageOpen(PCIMAGE_ImageInfo lpImageInfo)
{
	Word32 wFrgb;
	Word32 wSrgb;
	Handle hDescDIB;
	//CTDIBRGBQUAD tmpQ;

#if defined( _TRASING ) & defined ( _DEBUG )
	if ( wCBStep != 0 )
	{
		TRACE("!!!CIMAGE!!! ��������� ����� CBImageOpen ��� �������� %s ��� ����������������� CIMAGE_CBImageRead!\n",mCBName);
	}
#endif

	wCBStep++;

#if defined( _TRASING ) & defined ( _DEBUG )
	if ( mCBWInProcess == TRUE )
	{
		TRACE("!!!CIMAGE!!! CIMAGE_CBImageRead( %s, ...) ���������� �� CIMAGE_WriteCBImage( %s, ...)!\n",mCBName,mCBWName);
	}
#endif


	if ( !hCBImage )
	{
		return FALSE;
	}

	pCBImage = CIMAGELock(hCBImage);

	if ( mCBDestianationDIB != NULL || mCBSourceDIB != NULL )
	{
		return FALSE;
	}

	mCBSourceDIB       = new CTDIB;
	mCBDestianationDIB = new CTDIB;

	if ( mCBSourceDIB->SetDIBbyPtr(pCBImage) )
	{
		wCBBufferSize = mCBSourceDIB->GetUsedLineWidthInBytes();
		wCBWidth      = mCBSourceDIB->GetLineWidth();
		wCBStep++;

		CIMAGEComment("Temporary DIB - CBImageOpen");
		if ( mCBDestianationDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) &&
			 (hDescDIB = mCBDestianationDIB->CreateDIBBegin(wCBWidth,1,mCBSourceDIB->GetPixelSize())) &&
			 mCBDestianationDIB->CreateDIBEnd() )
		{
			wCBLines	= mCBSourceDIB->GetLinesNumber();

			lpImageInfo->wImageWidth        = (Word16)mCBSourceDIB->GetLineWidth();
			lpImageInfo->wImageHeight       = (Word16)wCBLines;
			lpImageInfo->wImageByteWidth    = (Word16)wCBBufferSize;
			lpImageInfo->wImageDisplacement = 0;

			mCBSourceDIB->GetResolutionDPI(&wFrgb, &wSrgb);
			lpImageInfo->wResolutionX       = (Word16)wFrgb;
			lpImageInfo->wResolutionY       = (Word16)wSrgb;

			lpImageInfo->bFotoMetrics       = (Word8)mCBSourceDIB->GetWhitePixel();

			lpImageInfo->wAddX              = 0;
			lpImageInfo->wAddY              = 0;

			wCBStep++;
		}
	}


	wCBLine = 0;

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Word32 CTIControl::CBImageRead(PChar8  lpBuff, Word32 wMaxSize)
{
	CIMAGE_InfoDataInGet   InFrame;
	Word32                 LinesAtOnce;
	Word32                 nOutLine;
	Word32                 CopiedBytes = 0;
	PChar8                 pNextLineInBuffer = lpBuff;

#ifndef CIMAGE_CBR_ONE_LINE
	LinesAtOnce = wMaxSize / wCBBufferSize;
#else
	LinesAtOnce = 1;
#endif

	if ( pCBImage )
	{
		if ( !(wMaxSize < wCBBufferSize) )
		{
			InFrame.dwX        = 0;
			InFrame.dwWidth    = mCBDestianationDIB->GetLineWidth();
			InFrame.dwHeight   = mCBDestianationDIB->GetLinesNumber();
			InFrame.wByteWidth = (Word16)(wCBBufferSize);
			InFrame.MaskFlag   = 0;

			for ( nOutLine = 0;nOutLine < LinesAtOnce; nOutLine++ )
			{
				InFrame.dwY        = wCBLine;

				if ( mCBDestianationDIB->GetPtrToLine(0) != NULL )
				{
					if ( GetFrame(mCBSourceDIB, mCBDestianationDIB, &InFrame, NULL) )
					{
						wCBLine++;
						wCBStep+=10;
						memcpy(pNextLineInBuffer, mCBDestianationDIB->GetPtrToLine(0), wCBBufferSize );
						CopiedBytes+=wCBBufferSize;
						pNextLineInBuffer += wCBBufferSize;
					}
				}
			}
			return CopiedBytes;
		}
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CBImageClose(void)
{
	if ( hCBImage )
		CIMAGEUnlock(hCBImage);

	if ( pCBBuffer )
	{
		delete [] pCBBuffer;
		pCBBuffer = NULL;
	}

	if ( mCBSourceDIB )
	{
		delete mCBSourceDIB;
		mCBSourceDIB = NULL;
	}

	if ( mCBDestianationDIB )
	{
		mCBDestianationDIB->DestroyDIB();
		delete mCBDestianationDIB;
		mCBDestianationDIB = NULL;
	}

	hCBImage = NULL;
	pCBBuffer = NULL;
	wCBBufferSize = 0;
	wCBLines = 0;
	wCBLine  = 0;
	wCBWidth = 0;
	wCBStep  = 0;

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::FreeAlloced(Handle hDIB)
{
	if (hDIB == NULL)
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( mlImages.FindHandle(hDIB) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
		return FALSE;
	}

	if ( hDIB == mhCopyedDIB )
		mhCopyedDIB = NULL;

	CIMAGEFree(hDIB);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::DumpToFile(PChar8 FileName, PWord8 pData, Word32 Size)
{
	FILE * fDump;

	fDump = fopen(FileName, "wb");

	if ( fDump == NULL )
		return FALSE;

	fwrite(pData, Size, 1, fDump);
	fclose(fDump);
	return TRUE;
}

Bool32 CTIControl::GetDIBFromImage(PChar8 lpName, PCIMAGE_InfoDataInGet lpIn, PInt8 *pDIB)
{
	Handle hImage = NULL;
	void * pImage = NULL;
	Bool32   bRet = FALSE;
	PCTDIB   pSrcDIB;
	Word32   wResolutionX;
	Word32   wResolutionY;
	CIMAGE_InfoDataInGet NewIn;

	// � ������ ���������� ������ ���������� ��� ���������
	FreeBuffers();

	// ��� ������ ������
	//����� ��������� �� ��������
	if ( !GetDIB(lpName, &hImage, TRUE ) )
	{
		return bRet;

	}

	pImage = CIMAGELock(hImage);

	if ( !pImage )
	{
		SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
		return bRet;
	}

	//�������������� ctdib-����
	pSrcDIB = new CTDIB;
	mpDIBFromImage = new CTDIB;

	if ( pSrcDIB->SetDIBbyPtr(pImage) &&
		 pSrcDIB->GetResolutionDPM(&wResolutionX, &wResolutionY) )
	{
		// ��������� ������� ��������
		if ( CheckInData(pSrcDIB, lpIn, &NewIn) )
		{

			if ( mpDIBFromImage->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock) )
			{
				CIMAGEComment("Temporary DIB - GetDIBFromImage");
				if ( mpDIBFromImage->CreateDIBBegin(NewIn.dwWidth, NewIn.dwHeight,pSrcDIB->GetPixelSize(),pSrcDIB->GetActualColorNumber()) &&
					 mpDIBFromImage->CopyPalleteFromDIB( pSrcDIB ) &&
					 mpDIBFromImage->SetResolutionDPM(wResolutionX, wResolutionY) &&
					 mpDIBFromImage->CreateDIBEnd() )
				{
					bRet = GetFrame(pSrcDIB, mpDIBFromImage, &NewIn, (NewIn.MaskFlag == TRUE ? (PWord8)&lpIn[1] : NULL) );
				}
				else
					SetReturnCode_cimage(IDS_CIMAGE_UNABLE_CREATE_DIB);
			}
			else
				SetReturnCode_cimage(IDS_CIMAGE_UNABLE_COPY_DIB);
		}
		else
			SetReturnCode_cimage(IDS_CIMAGE_INVALID_PARAMETR);
	}

	pSrcDIB->DestroyDIB();
	delete pSrcDIB;

	WriteDIBtoBMP("Allex.DIBFromImage.bmp", mpDIBFromImage);

	if ( mpDIBFromImage->GetDIBPtr( (void**)pDIB ) )
		return TRUE;

	*pDIB = NULL;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file

Bool32 CTIControl::FreeBuffers()
{
	Bool32 bCrashedDIB = FALSE;

	if ( mpBitFildFromImage != NULL )
	{
		CIMAGEFree(mpBitFildFromImage);
		mpBitFildFromImage = NULL;
	}

	if ( mhCopyedDIB )
	{
		//CIMAGEFree(mhCopyedDIB);
		mhCopyedDIB = NULL;
	}

	if ( mpDIBFromImage != NULL )
	{
		if ( mpDIBFromImage->DestroyDIB() )
			delete mpDIBFromImage;
		else
		{
			bCrashedDIB = TRUE;
			mwMemoryErrors++;
		}

		mpDIBFromImage = NULL;
		// ���� �����, ��� �� ��� ������
		//if ( pDIB != NULL )
		//	*pDIB = NULL;
		//return TRUE;
	}

	return TRUE;
}

Bool32 CTIControl::CheckInData(PCTDIB pDIB, PCIMAGE_InfoDataInGet lpIn, PCIMAGE_InfoDataInGet lpNewIn)
{
	PInt32 NewX = (PInt32)(lpNewIn ? &lpNewIn->dwX : &lpIn->dwX);
	PInt32 NewY = (PInt32)(lpNewIn ? &lpNewIn->dwY : &lpIn->dwY);
	PInt32 NewH = (PInt32)(lpNewIn ? &lpNewIn->dwHeight : &lpIn->dwHeight);
	PInt32 NewW = (PInt32)(lpNewIn ? &lpNewIn->dwWidth  : &lpIn->dwWidth);
	Int32 DIBW;  //= (Int32)pDIB->GetLineWidth();
	Int32 DIBH;  //= (Int32)pDIB->GetLinesNumber();

	if ( pDIB == NULL )
		return FALSE;

	DIBW  = (Int32)pDIB->GetLineWidth();
	DIBH  = (Int32)pDIB->GetLinesNumber();

	// ��������
	if ( lpNewIn )
	{
		memcpy( lpNewIn, lpIn, sizeof(CIMAGE_InfoDataInGet) );
	}
	//��������� ������
	if ( *NewW  < 0 )
	{
		*NewW = DIBW;
	}
	// � ������
	if ( *NewH  < 0 )
	{
		*NewH = DIBH;
	}

	//��������� X
	if ( *NewX < 0 )
	{
		if ( *NewW > -*NewX )
			*NewW = *NewW + *NewX;

		*NewX = 0;
	}

	if ( *NewX > DIBW )
	{
		if ( *NewW < DIBW )
			*NewX = DIBW - *NewW;
		else
			*NewX = 0;
	}
	// ��������� Y
	if ( *NewY < 0 )
	{
		if ( *NewH > -*NewY )
			*NewH = *NewH + *NewY;

		*NewY = 0;
	}

	if ( *NewY > DIBH )
	{
		if ( *NewH < DIBH )
			*NewY = DIBH - *NewH;
		else
			*NewY = 0;
	}
	// ��������� �������
	if ( (*NewX + *NewW) > DIBW )
	{
		*NewW = DIBW - *NewX;
	}

	if ( (*NewY + *NewH) > DIBH )
	{
		*NewH = DIBH - *NewY;
	}

	return TRUE;
}

Bool32 CTIControl::WriteDIBtoBMP(const char *cName, PCTDIB pDIB)
{
#ifdef CIMAGE_DUMP_ENABLE

	Word32  wBMPSize = pDIB->GetDIBSize() + 14;
	PumaMemoryToFileDumper BMPDump(cName);

	BMPDump.AddDump("BM",2);
	BMPDump.AddDump((char *)&wBMPSize,4);
	wBMPSize = 0;
	BMPDump.AddDump((char *)&wBMPSize,4);
	wBMPSize = 14 + pDIB->GetHeaderSize() + pDIB->GetRGBPalleteSize();
	BMPDump.AddDump((char *)&wBMPSize,4);
	BMPDump.AddDump(pDIB->GetPtrToHeader(), pDIB->GetDIBSize());
	BMPDump.EndDump();

#endif

	return TRUE;
}

Bool32 CTIControl::AddWriteRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst)
{
	return AddRectsToMask(lpName, wNumber, pFirst, "w");
}

Bool32 CTIControl::RemoveWriteRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst)
{
	return RemoveRectsFromMask(lpName, wNumber, pFirst, "w");
}

Bool32 CTIControl::AddReadRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst)
{
	return AddRectsToMask(lpName, wNumber, pFirst, "r");
}

Bool32 CTIControl::RemoveReadRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst)
{
	return RemoveRectsFromMask(lpName, wNumber, pFirst, "r");
}

Bool32 CTIControl::OpenDIBFromList(PChar8 lpName, PHandle phImage)
{
	mlImages.GetImage(lpName, phImage);
	//ALLEX Mask
	// ������ �� ���
	OpenMaskFromList(lpName, &(mpcSrcDIBReadMask = NULL), &mbEnableDIBReadMask, "r");
	OpenMaskFromList(lpName, &(mpcSrcDIBWriteMask = NULL), &mbEnableDIBWriteMask, "w");

	if ( !(*phImage) )
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}

	return TRUE;
}

Bool32 CTIControl::OpenMaskFromList(const char *lpName, PPCTIMask ppMask, PBool32 pEnMask, const char *pcType)
{
	Bool32 bRet;

	if ( pcType[0] == 'r' )
		bRet = mlImages.GetImageReadMask(lpName, ppMask, pEnMask);
	else
		if ( pcType[0] == 'w' )
			bRet = mlImages.GetImageWriteMask(lpName, ppMask, pEnMask);

	if ( !bRet )
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);

	return bRet;
}

Bool32 CTIControl::SetMaskToList(PChar8 pName, PCTIMask pMask, PChar8 pcType)
{
	Bool32 bRet;

	if ( pcType[0] == 'r' )
		bRet = mlImages.SetImageReadMask(pName, pMask);
	else
		if ( pcType[0] == 'w' )
			bRet = mlImages.SetImageWriteMask(pName, pMask);

	if ( !bRet )
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);

	return bRet;
}

Bool32 CTIControl::OpenDIBFromList(const char *lpName, PCTDIB pcDIB)
{
	Handle hDIB;

	mlImages.GetImage(lpName, &hDIB);
	//ALLEX Mask
	// ������ �� ���
	OpenMaskFromList(lpName, &(mpcSrcDIBReadMask = NULL), &mbEnableDIBReadMask, "r");
	OpenMaskFromList(lpName, &(mpcSrcDIBWriteMask = NULL), &mbEnableDIBWriteMask, "w");

	if ( !hDIB )
	{
		SetReturnCode_cimage(IDS_CIMAGE_NO_IMAGE_FOUND);
		return FALSE;
	}
	pcDIB->SetDIBHandle(hDIB);
	return pcDIB->SetDIBbyPtr(CIMAGELock(hDIB));
}

Bool32 CTIControl::CloseDIBFromList(PCTDIB pDIB)
{
	Handle hDIB;

	if ( pDIB->GetDIBHandle( &hDIB ) )
		CIMAGEUnlock(hDIB);

	pDIB->DestroyDIB();

	return TRUE;
}

Bool32 CTIControl::AddRectsToMask(const char *lpName, Word32 wNumber, PCIMAGE_Rect pFirstRect, const char *pcType)
{
	Bool32 bRet = FALSE;
	PCTIMask pMask;
	Bool32  bInvalidMask = FALSE;
	Word32  wRect;
	Bool32  bMaskEnabled;

	if ( !OpenMaskFromList(lpName, &pMask, &bMaskEnabled, pcType) )
	{
		return FALSE;
	}

	if ( pMask == NULL )
	{
		CTDIB   DIB;

		if ( !OpenDIBFromList(lpName, &DIB) )
			return FALSE;

		pMask = new CTIMask(DIB.GetLineWidth(), DIB.GetLinesNumber());
		CloseDIBFromList(&DIB);

		if ( !mlImages.SetImageReadMask(lpName, pMask) )
		{
			delete pMask;
			return FALSE;
		}
	}

	for ( wRect = 0; wRect < wNumber; wRect++ )
	{
		if ( pMask->AddRectangle(&pFirstRect[wRect]) )
			continue;

		bInvalidMask = TRUE;
		break;
	}

	bRet = (wRect == wNumber);
	return bRet;
}

Bool32 CTIControl::RemoveRectsFromMask(const char *lpName, Word32 wNumber, PCIMAGE_Rect pFirstRect, const char *pcType)
{
	Bool32 bRet = true;
	PCTIMask pMask;
	Bool32  bInvalidMask = FALSE;
	Word32  wRect;
	Bool32  bMaskEnabled;

	if ( !OpenMaskFromList(lpName, &pMask, &bMaskEnabled, pcType) )
	{
		return FALSE;
	}

	if ( pMask == NULL )
	{
		return FALSE;
	}

	for ( wRect = 0; wRect < wNumber; wRect++ )
	{
		if ( pMask->RemoveRectangle(&pFirstRect[wRect]) )
			continue;

		bInvalidMask = TRUE;
		break;
	}

	return bRet;
}

Bool32 CTIControl::ApplayMaskToDIB(PCTDIB pDIB, PCTIMask pMask, Word32 wAtX, Word32 wAtY)
{
	Word32 wX, wY;
	Word32 wXb, wXe;
	Word32 wYb, wYe;
	Word32 wSegmentsOnLine;
	PCTIMaskLine  pcMaskLine;

	if ( !pDIB )
		return FALSE;

	wXe = (wXb = wAtX) + pDIB->GetLineWidth();
	wYe = (wYb = wAtY) + pDIB->GetLinesNumber();

	CIMAGE_Rect cRectAllDIB = {wXb, wYb, wXe - wXb, wYe - wYb};

	if ( !pMask->IsRectOnMask( &cRectAllDIB ) )
		return FALSE;

	for ( wY = wYb; wY < wYe; wY++ )
	{
		//���� ������� ������� �����
		if ( pMask->GetLine(wY, &pcMaskLine ) )
		{
			// ���� ���� �����
			if ( pcMaskLine )
			{
				wSegmentsOnLine = pcMaskLine->GetSegmentsNumber();
				wX = wXb;

				// ���� ���� �������� �� ���� �����
				if ( wSegmentsOnLine )
					while ( wX < wXe )
					{
						CTIMaskLineSegment Segm(wX, wXe);

						if ( pcMaskLine->GetLeftIntersection( &Segm ) )
						{
							wX = Segm.GetEnd() + 1;
							if ( !ApplayMaskToDIBLine( pDIB, &Segm, wY, wAtX, wAtY) )
							{
								SetReturnCode_cimage(IDS_CIMAGE_UNABLE_APPLAY_MASK);
								return FALSE;
							}
						}
						else
							break;
					}
			}
		}
	}

	return TRUE;
}

Bool32 CTIControl::ApplayMaskToDIBLine(PCTDIB pcDIB, PCTIMaskLineSegment pSegm, Word32 wLine, Word32 wAtX, Word32 wAtY)
{
	Word32 wBitCount;
	Bool32 bRet = FALSE;
	Word32 wXb;
	Word32 wXe;
	Word32 wY;
	PWord8 pPixB;
	PWord8 pPixE;
	Word8  wWPLA;
	Word32 wSegmLenght;
	Word32 wLAStartPos;
	Word32 wLAEndPos;
	Int32 wLAFullBits;

	if ( ( pSegm->GetStart() > (Int32)(pcDIB->GetLineWidth() + wAtX )) ||
		 ( pSegm->GetEnd() > (Int32)(pcDIB->GetLineWidth() + wAtX )) ||
		 ( wLine >= pcDIB->GetLinesNumber() + wAtY ) )
		 return FALSE;

	wBitCount = pcDIB->GetPixelSize();
	wXe = pSegm->GetEnd() - wAtX;
	wXb = pSegm->GetStart() - wAtX;
	wY  = wLine - wAtY;

	// ��������� �� ������������ � ���������
	if ( wXb >= pcDIB->GetLineWidth() )
		return true;

	if ( wXe >= pcDIB->GetLineWidth() )
		wXe = pcDIB->GetLineWidth() - 1;

	pPixB = (PWord8)pcDIB->GetPtrToPixel(wXb, wY);
	pPixE = (PWord8)pcDIB->GetPtrToPixel(wXe, wY);

	if ( pPixB == NULL || pPixE == NULL )
	{
		return bRet;
	}


	switch ( wBitCount )
	{
	case 1:
		wLAStartPos = pcDIB->GetPixelShiftInByte(wXb);
		wLAEndPos   = pcDIB->GetPixelShiftInByte(wXe);
		wWPLA       = (Word8)pcDIB->GetWhitePixel();
		wLAFullBits = pPixE - pPixB - 1;

		if ( wLAFullBits > 0 )
		{
			if ( wWPLA == 0 )
				CIMAGE_MEMSET(pPixB + 1, 0x00, wLAFullBits );
			else
				CIMAGE_MEMSET(pPixB + 1, 0xff, wLAFullBits );
		}

		if ( wWPLA == 0 )
		{
			*pPixB = *pPixB & mwLABlackRightMask[wLAStartPos];
			*pPixE = *pPixE & mwLABlackLeftMask[wLAEndPos];
		}
		else
		{
			*pPixB = *pPixB | mwLAWhiteRightMask[wLAStartPos];
			*pPixE = *pPixE | mwLAWhiteLeftMask[wLAEndPos];
		}
		bRet = true;
		break;
	case 4:
		break;
	case 8:
	case 16:
	case 24:
	case 32:
		wSegmLenght = ((pSegm->GetEnd() - pSegm->GetStart() ) * wBitCount ) / 8;
		CIMAGE_MEMSET( pPixB, (Word8)pcDIB->GetWhitePixel(), wSegmLenght );
		bRet = TRUE;
		break;
		//wSegmLenght = ((pSegm->GetEnd() - pSegm->GetStart() + 1) * wBitCount ) / 8;
		//wWP24 = (Word8)pcDIB->GetWhitePixel();
		//memset( pPix, wWP24, wSegmLenght );
		//bRet = TRUE;
		//break;
	}

	return bRet;
}

Bool32 CTIControl::EnableMask(PChar8 pcName, PChar8 pcType, Bool32 bEnable)
{
	return mlImages.EnableMask(pcName, pcType, bEnable);
}
