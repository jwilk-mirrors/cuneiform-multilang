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



/****************************************************************************
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTPAGE.C - Working with PageMatrix for initial breaking     *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
/*# include <crtdbg.h>*/
#include <assert.h>

# include "c_memory.h"
# include "layout.h"
# include "struct.h"
# include "msgerr.h"

# include "dpuma.h"
#include "minmax.h"
#include "compat_defs.h"

BYTE * PageMatrix;  ////  [PAGE_MATRIX_SIZE];
extern BYTE *CellsPage;
extern Handle hPageMatrix;

BOOL bPageMatrixInitialized = FALSE;

void PageMatrixInit (int nWidth, int nHeight)
{
#ifdef  HUGE_IMAGE
if( PageMatrix==NULL )
    PageMatrix = malloc(PAGE_MATRIX_SIZE);
    if(PageMatrix==NULL)
		error_exit(ERR_comp,13);
	memset (PageMatrix, PMC_NULL, PAGE_MATRIX_SIZE); //********* Rom 08-02-99
#else
    PageMatrix = CellsPage;
#endif
    if (nWidth  > XY_UNCOMPRESS (PAGE_MATRIX_WIDTH) ||
        nHeight > XY_UNCOMPRESS (PAGE_MATRIX_HEIGHT))
    {
        return;
    }

//    memset (PageMatrix, PMC_NULL, PAGE_MATRIX_SIZE); //********* Rom 08-02-99
    bPageMatrixInitialized = TRUE;
}

static void PutInterval (int y, int x, int l)
{
    memset
    (
        PageMatrix
          + (XY_COMPRESS (y) << PAGE_MATRIX_WIDTH_SHIFT) +  XY_COMPRESS (x),
        PMC_PICTURE,
        XY_COMPRESS (l + (x & PAGE_COMP_MASK) + PAGE_COMP_FACTOR - 1)
    );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AK 17.02.98
void SpecCompPut (MN *mn)
{
    BOX    *pBox;
    int    nBox;
    LNSTRT *pLine;

    BOXINT *pInts;
    BOXINT *pAfterInts;
    BOXINT *pInt;

    int xEnd = 0, y = 0;

    if (! bPageMatrixInitialized)
        return;

    for ( pBox = (BOX *) mn -> mnfirstbox, nBox = 0;
          nBox < mn -> mnboxcnt;
          pBox = pBox -> boxnext, nBox++)
    {
		// AK add c/g
        if ( !pBox->boxnext )
			return;

		if (pBox -> boxflag & BOXBEG)
        {
            pLine = (LNSTRT *) ((BYTE *) pBox + sizeof (BOX));

            xEnd = pLine -> x;
            y    = pLine -> y;

            PutInterval (y, xEnd - pLine -> l, pLine -> l);
            pInts = (BOXINT *) ((BYTE *) pBox + sizeof (BOX) + sizeof (LNSTRT));
        }
        else
        {
            if ( y == 0 )
				return;
			pInts = (BOXINT *) ((BYTE *) pBox + sizeof (BOX));
        }

        pAfterInts = (BOXINT *) ((BYTE *) pBox + pBox -> boxptr);

        for (pInt = pInts;
                 (BYTE *) pAfterInts - (BYTE *) pInt >= sizeof (BOXINT);
                     pInt++)
        {
            xEnd += pInt -> d;
            y++;
            PutInterval (y, xEnd - pInt -> l, pInt -> l);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void PageMatrixPutRoots (void)
{
    ROOT *p;
    RECTANGLE r;
    int o;

    for (p = pRoots; p < pAfterRoots; p++)
    {
        if (IS_LAYOUT_DUST (*p))
            continue;

        // �������� -- ��� ������� �� 2^N, N==4(==PAGE_COMP_SHIFT)
        r.xLeft   = XY_COMPRESS (p -> xColumn);
        r.xRight  = XY_COMPRESS (p -> xColumn + p -> nWidth - 1);
        r.yTop    = XY_COMPRESS (p -> yRow);
        r.yBottom = XY_COMPRESS (p -> yRow + p -> nHeight - 1);

        // "�����": ��� PAGE_MATRIX_WIDTH==1024
        // PAGE_MATRIX_WIDTH_SHIFT==10 (�������)
        for (o = r.yTop << PAGE_MATRIX_WIDTH_SHIFT;
                 o <= r.yBottom << PAGE_MATRIX_WIDTH_SHIFT;
                     o += PAGE_MATRIX_WIDTH)
        // �.�., for(y=top;y<=bottom;y++,o=PAGE_MATRIX_WIDTH*y)
        {
            BYTE *p;
			_ASSERT((o + r.xRight) < PAGE_MATRIX_SIZE); // < 1024^2
            for (p = PageMatrix + o + r.xLeft; // start=&matrix+MaxW*y+left
                     p <= PageMatrix + o + r.xRight; //end=start+right
                         p++)
            {   // ��������� ���� ������������� ��� "��������"
                *p |= PMC_ROOT;
            }
        }
    }
}

# define HORZ_GLUE_DIST  8
# define VERT_GLUE_DIST  (4 * PAGE_MATRIX_WIDTH)

// ������� �� �: ��� (�������) ��������� �������� ��������� �� ����� HORZ_GLUE_DIST
// (== 8 == ������ �������, ������) ���������. (�.�., ���������� ��������� ����� ����)
// ���������. �� ���������� ��� �������� ��������� �������
// �������� �������� ��-��� (������ � PAGE_COMP_FACTOR == (1 << PAGE_COMP_SHIFT) ���).
// ����� �������� ���������� ������� - 16*8==2^(4+2)==64,
// ��� ���� �� "��������" (���������) ����� 16��-�������� ������������ ��������.
void PageMatrixHorzGlue (void)
{
    int o, x;
    int xLast;
    int xLastPicture;

    for (o = 0; o < PAGE_MATRIX_SIZE; o += PAGE_MATRIX_WIDTH)
    {
        xLast    = -1;
        xLastPicture = -2;

        for (x = 0; x < PAGE_MATRIX_WIDTH; x++)
        {
            if (PageMatrix [o + x] & PMC_PICTURE)
            {
                xLastPicture = x;
            }

            if (PageMatrix [o + x] & PMC_ROOT)
            {
//                if (xLastPicture < xLast && x - xLast <= HORZ_GLUE_DIST)
                if (xLast>=0 && xLastPicture < xLast && x - xLast <= HORZ_GLUE_DIST) // Piter
                {
                    for (xLast++; xLast < x; xLast++)
                        PageMatrix [o + xLast] |= PMC_ROOT;
                }
                else
                {
                    xLast = x;
                }
            }
        }

        //if (xLastPicture < xLast && x - xLast <= HORZ_GLUE_DIST)
		if (xLast>=0 && xLastPicture < xLast && x - xLast <= HORZ_GLUE_DIST) // Piter
        {
            for (xLast++; xLast < x; xLast++)
                PageMatrix [o + xLast] |= PMC_ROOT;
        }
    }
}

// ����������� ��� ������������ ���������, �������������� ����������
// ����� �������� �� ����� VERT_GLUE_DIST/PAGE_MATRIX_WIDTH (==4)
// (� ������� PageMatrix ��� ���������� ���������� �������� ������ �� PAGE_MATRIX_WIDTH)
// ���������. ����� PageMatrix �������� �� "�����" ������� � PAGE_COMP_FACTOR == (1 << PAGE_COMP_SHIFT) ���.
// ����� �������� ���������� ������� - 16*4==2^(4+2)==64, ��� ���� �� �������� ����� 16��-�������� ��������
void PageMatrixVertGlue (void)
{
    int x, o;
    int oLast;
    int oLastPicture;

    for (x = 0; x < PAGE_MATRIX_WIDTH; x++)
    {
        oLast        = x - PAGE_MATRIX_WIDTH;
        oLastPicture = x - PAGE_MATRIX_WIDTH * 2;

        for (o = x; o < PAGE_MATRIX_SIZE; o += PAGE_MATRIX_WIDTH)
        {
            if (PageMatrix [o] & PMC_PICTURE)
            {
                oLastPicture = o;
            }

            if (PageMatrix [o] & PMC_ROOT)
            {
               // if (oLastPicture < oLast && o - oLast <= VERT_GLUE_DIST)
				 if (oLast >=0 && oLastPicture < oLast && o - oLast <= VERT_GLUE_DIST) // Piter
                {
                    for (oLast += PAGE_MATRIX_WIDTH;
                             oLast < o;
                                 oLast += PAGE_MATRIX_WIDTH)
                    {
                        PageMatrix [oLast] |= PMC_ROOT;
                    }
                }
                else
                {
                    oLast = o;
                }
            }
        }

        //if (oLastPicture < oLast && o - oLast <= VERT_GLUE_DIST)
		if (oLast >=0 && oLastPicture < oLast && o - oLast <= VERT_GLUE_DIST)
        {
            for (oLast += PAGE_MATRIX_WIDTH;
                        oLast < o;
                            oLast += PAGE_MATRIX_WIDTH)
            {
                PageMatrix [oLast] |= PMC_ROOT;
            }
        }
    }
}

// ���� "����" ("���������" (����������) �����) ��������� � ������ ����� ��������
void PageMatrixExcludeIntersections (void)
{
    int o;

    for (o = 0; o < PAGE_MATRIX_SIZE; o++)
    {
        if ((PageMatrix [o] & PMC_ROOT) &&
            (PageMatrix [o] & PMC_PICTURE))
        {
            PageMatrix [o] &= ~PMC_ROOT;
        }
    }
}

void PageMatrixBuild (void)
{
	PageMatrixInit (3000, 3000);//DDD
    PageMatrixPutRoots ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if (!LDPUMA_Skip(hPageMatrix))
        LT_GraphicsPageMatrixOutput ("After put roots");
# endif

    PageMatrixHorzGlue ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if (!LDPUMA_Skip(hPageMatrix))
        LT_GraphicsPageMatrixOutput ("After horz glue");
# endif

    PageMatrixVertGlue ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if (!LDPUMA_Skip(hPageMatrix))
        LT_GraphicsPageMatrixOutput ("After vert glue");
# endif

    PageMatrixExcludeIntersections ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if (!LDPUMA_Skip(hPageMatrix))
        LT_GraphicsPageMatrixOutput ("After excluding intersections");
# endif

    PageMatrixPutSeparators (FALSE); //BOOL bConvertToRealCoords:=FALSE

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if (!LDPUMA_Skip(hPageMatrix))
        LT_GraphicsPageMatrixOutput ("After put separators");
# endif
}

typedef void (*DRAW_ACTION) (int, int, int);

static int nExtension;
static DRAW_ACTION fDrawMatrixPictureHorzInterval;
static DRAW_ACTION fDrawMatrixPictureVertInterval;

void PutMatrixPictureHorzInterval (int y, int x1, int x2)
{
    BYTE *p, *pBegin, *pEnd;

    if (y < 0 || y >= PAGE_MATRIX_HEIGHT)
        return;

    if (x1 > x2)
        EXCHANGE_INTS (x1, x2);

    if (x1 >= PAGE_MATRIX_WIDTH)
        return;

    if (x1 < 0)
        x1 = 0;

    if (x2 >= PAGE_MATRIX_WIDTH)
        x2 = PAGE_MATRIX_WIDTH - 1;

    pBegin = PageMatrix + (y << PAGE_MATRIX_WIDTH_SHIFT) + x1;
    pEnd   = PageMatrix + (y << PAGE_MATRIX_WIDTH_SHIFT) + x2;

    for (p = pBegin; p <= pEnd; p++)
        *p |= PMC_PICTURE;
}

// ������������ ���������� ���������� ����������� ����� ��������
void PutMatrixPictureVertInterval (int x, int y1, int y2)
{
    BYTE *p, *pBegin, *pEnd;

    if (x < 0 || x >= PAGE_MATRIX_WIDTH)
        return;

    if (y1 > y2)
        EXCHANGE_INTS (y1, y2);

    if (y1 >= PAGE_MATRIX_HEIGHT)
        return;

    if (y1 < 0)
        y1 = 0;

    if (y2 >= PAGE_MATRIX_HEIGHT)
        y2 = PAGE_MATRIX_HEIGHT - 1;

    pBegin = PageMatrix + (y1 << PAGE_MATRIX_WIDTH_SHIFT) + x;
    pEnd   = PageMatrix + (y2 << PAGE_MATRIX_WIDTH_SHIFT) + x;

    for (p = pBegin; p <= pEnd; p += PAGE_MATRIX_WIDTH)
        *p |= PMC_PICTURE;
}

void RemoveMatrixPictureHorzInterval (int y, int x1, int x2)
{
    BYTE *p, *pBegin, *pEnd;

    if (y < 0 || y >= PAGE_MATRIX_HEIGHT)
        return;

    if (x1 > x2)
        EXCHANGE_INTS (x1, x2);

    if (x1 >= PAGE_MATRIX_WIDTH)
        return;

    if (x1 < 0)
        x1 = 0;

    if (x2 >= PAGE_MATRIX_WIDTH)
        x2 = PAGE_MATRIX_WIDTH - 1;

    pBegin = PageMatrix + (y << PAGE_MATRIX_WIDTH_SHIFT) + x1;
    pEnd   = PageMatrix + (y << PAGE_MATRIX_WIDTH_SHIFT) + x2;

    for (p = pBegin; p <= pEnd; p++)
        *p &= ~PMC_PICTURE;
}

void RemoveMatrixPictureVertInterval (int x, int y1, int y2)
{
    BYTE *p, *pBegin, *pEnd;

    if (x < 0 || x >= PAGE_MATRIX_WIDTH)
        return;

    if (y1 > y2)
        EXCHANGE_INTS (y1, y2);

    if (y1 >= PAGE_MATRIX_HEIGHT)
        return;

    if (y1 < 0)
        y1 = 0;

    if (y2 >= PAGE_MATRIX_HEIGHT)
        y2 = PAGE_MATRIX_HEIGHT - 1;

    pBegin = PageMatrix + (y1 << PAGE_MATRIX_WIDTH_SHIFT) + x;
    pEnd   = PageMatrix + (y2 << PAGE_MATRIX_WIDTH_SHIFT) + x;

    for (p = pBegin; p <= pEnd; p += PAGE_MATRIX_WIDTH)
        *p &= ~PMC_PICTURE;
}

void DrawMatrixPictureRectangle (RECTANGLE r)
{
    int y;

    if (r.yTop > r.yBottom)
        EXCHANGE_INTS (r.yTop, r.yBottom);

    if (r.xLeft > r.xRight)
        EXCHANGE_INTS (r.xLeft, r.xRight);

    for (y = r.yTop; y <= r.yBottom; y++)
        fDrawMatrixPictureHorzInterval (y, r.xLeft, r.xRight);
}

void DrawRealHorzPictureLine (LPOINT Point1, LPOINT Point2, int nWidth)
{
    int y1, y2;
    int x, y;
    int nHalfWidth = MAX (1, nWidth / 2);
    int dx, dy;
    LPOINT LocalBegin, LocalEnd;

    // ����� ������ -- ���� �������� ��������� � ��������� �� 16 �����,
    // �� "��������" ����� �������������� ���������� ������(/16) �������
    // (�� �� ����� ���������, ����)
    if (XY_COMPRESS (Point1.y) == XY_COMPRESS (Point2.y))
    {
        y1 = XY_COMPRESS (MIN (Point1.y, Point2.y) - nHalfWidth)
             - nExtension;
        y2 = XY_COMPRESS (MAX (Point1.y, Point2.y) + nHalfWidth)
             + nExtension;

        for (y = y1; y <= y2; y++)
        {
            fDrawMatrixPictureHorzInterval
                (y, XY_COMPRESS (Point1.x), XY_COMPRESS (Point2.x));
        }
        return;
    }

    // else "if (XY_COMPRESS (Point1.y) != XY_COMPRESS (Point2.y))" :
    if (Point1.x > Point2.x)
    {
        EXCHANGE_INTS (Point1.x, Point2.x);
        EXCHANGE_INTS (Point1.y, Point2.y);
    }

    dx = Point2.x - Point1.x;
    dy = Point2.y - Point1.y;

    LocalEnd.x = XY_UNCOMPRESS (XY_COMPRESS (Point1.x));
#ifndef NO_GEORGE
    //LocalEnd.y = Point1.y + LocalEnd.x * dy / dx;
    LocalEnd.y = Point1.y; //George
    // ����� ������, ������������ ����������
    // �� ������� ������������� ���������
#else
    LocalEnd.y = Point1.y + LocalEnd.x * dy / dx;
#endif
    for (x = LocalEnd.x; x <= Point2.x; x += PAGE_COMP_FACTOR)
    {
        LocalBegin = LocalEnd;

        LocalEnd.x += PAGE_COMP_FACTOR;
#ifndef NO_GEORGE
        //LocalEnd.y = Point1.y + LocalEnd.x * dy / dx;
        LocalEnd.y = Point1.y + (LocalEnd.x-LocalBegin.x) * dy / dx; //George
        // � �����, ��� Point1.x=0 ��� ���� �� ������� ������������� ���������������.
        // ������ ��� ��� ��, �� �������� �� ������, �� ������� ����� �� ���������
        // � ����� Point1.x, ������ �� ���� (�� ����, � �����, �� ����� Point1.x * dy/dx)
#else
        LocalEnd.y = Point1.y + LocalEnd.x * dy / dx;
#endif
        y1 = XY_COMPRESS (MIN (LocalBegin.y, LocalEnd.y) - nHalfWidth)
             - nExtension;
        y2 = XY_COMPRESS (MAX (LocalBegin.y, LocalEnd.y) + nHalfWidth)
             + nExtension;

        fDrawMatrixPictureVertInterval (XY_COMPRESS (x), y1, y2);
    }
}

void DrawRealVertPictureLine (LPOINT Point1, LPOINT Point2, int nWidth)
{
    int x1, x2;
    int x;
    int nHalfWidth = MAX (1, nWidth / 2);

    if (XY_COMPRESS (Point1.x) == XY_COMPRESS (Point2.x))
    {
        x1 = XY_COMPRESS (MIN (Point1.x, Point2.x) - nHalfWidth);
        x2 = XY_COMPRESS (MAX (Point1.x, Point2.x) + nHalfWidth);

        for (x = x1; x <= x2; x++)
        {
            fDrawMatrixPictureVertInterval
                (x, XY_COMPRESS (Point1.y), XY_COMPRESS (Point2.y));
        }
    }
}

void DrawRealPictureLine (LPOINT Point1, LPOINT Point2, int nWidth)
{
    int nHalfWidth = MAX (1, nWidth / 2);
    RECTANGLE rBegin, rEnd;

    rBegin.xLeft   = XY_COMPRESS (Point1.x - nHalfWidth);
    rBegin.yTop    = XY_COMPRESS (Point1.y - nHalfWidth);
    rBegin.xRight  = XY_COMPRESS (Point1.x + nHalfWidth);
    rBegin.yBottom = XY_COMPRESS (Point1.y + nHalfWidth);

    rEnd.xLeft     = XY_COMPRESS (Point2.x - nHalfWidth);
    rEnd.yTop      = XY_COMPRESS (Point2.y - nHalfWidth);
    rEnd.xRight    = XY_COMPRESS (Point2.x + nHalfWidth);
    rEnd.yBottom   = XY_COMPRESS (Point2.y + nHalfWidth);

    // ���������� ���������� (nWidth/2) - ����������� ����� ������ � ����� ����� (??)
    DrawMatrixPictureRectangle (rBegin);
    DrawMatrixPictureRectangle (rEnd);

    if (Point1.x == Point2.x && Point1.y == Point2.y)
        return;
    else if (abs (Point1.x - Point2.x) >= abs (Point1.y - Point2.y))
        DrawRealHorzPictureLine (Point1, Point2, nWidth);
    else
        DrawRealVertPictureLine (Point1, Point2, nWidth);
}

void PutRealPictureLine (LPOINT Point1, LPOINT Point2, int nWidth)
{
    nExtension = 0; // ��������� ����� ��� ��������� � ������� ��������;
    // ������ (23.10.01) -- 0 ���� 1;

    // ��������� (����������) ��������� �� �������
    fDrawMatrixPictureHorzInterval = PutMatrixPictureHorzInterval;
    fDrawMatrixPictureVertInterval = PutMatrixPictureVertInterval;

    DrawRealPictureLine (Point1, Point2, nWidth);
}

void RemoveRealPictureLine (LPOINT Point1, LPOINT Point2, int nWidth)
{
    nExtension = 1;
    fDrawMatrixPictureHorzInterval = RemoveMatrixPictureHorzInterval;
    fDrawMatrixPictureVertInterval = RemoveMatrixPictureVertInterval;
    DrawRealPictureLine (Point1, Point2, nWidth);
}

void PageMatrixPutSeparators (BOOL bConvertToRealCoords)
{
    LPOINT Begin, End;
    int i;

    for (i = 0; i < nSeps; i++)
    {
        switch (pSeps [i].Type)
        {
            case SEP_VERT:
            case SEP_HORZ:
                Begin.x = pSeps [i].xBegin;
                Begin.y = pSeps [i].yBegin;
                End.x   = pSeps [i].xEnd;
                End.y   = pSeps [i].yEnd;

                // "��������" (�����) ���� �������
                if (bConvertToRealCoords)
                {
                    REAL_XY (Begin.x, Begin.y);
                    REAL_XY (End.x,   End.y);
                }

                PutRealPictureLine (Begin, End, pSeps [i].nWidth);
                break;

            default:
                break;
        }
    }
}

void PageMatrixExcludeSeparators (BOOL bConvertToRealCoords)
{
    LPOINT Begin, End;
    int i;

    for (i = 0; i < nSeps; i++)
    {
        switch (pSeps [i].Type)
        {
            case SEP_VERT:
            case SEP_HORZ:
                Begin.x = pSeps [i].xBegin;
                Begin.y = pSeps [i].yBegin;
                End.x   = pSeps [i].xEnd;
                End.y   = pSeps [i].yEnd;

                if (bConvertToRealCoords)
                {
                    REAL_XY (Begin.x, Begin.y);
                    REAL_XY (End.x,   End.y);
                }

                RemoveRealPictureLine (Begin, End, pSeps [i].nWidth);
                break;

            default:
                break;
        }
    }
}

BYTE PageMatrixFlagsByIdealXY (int xIdeal, int yIdeal)
{
    int xReal, yReal;

    if (! bPageMatrixInitialized)
        return (FALSE);

    xReal = xIdeal;
    yReal = yIdeal;

    REAL_XY (xReal, yReal);

    xReal = MIN (MAX (xReal, 0), PAGE_MATRIX_REAL_WIDTH  - 1);
    yReal = MIN (MAX (yReal, 0), PAGE_MATRIX_REAL_HEIGHT - 1);

    return (PageMatrix
            [
                XY_COMPRESS (xReal)
                + (XY_COMPRESS (yReal) << PAGE_MATRIX_WIDTH_SHIFT)
            ]);
}

void PageMatrixFreeData (void)
{
    bPageMatrixInitialized = FALSE;
#ifdef  HUGE_IMAGE
	free(PageMatrix);
	PageMatrix = NULL;
#endif
}
