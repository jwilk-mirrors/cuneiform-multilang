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

////////////////////////////////////////////////////////////
// CONTAINER OF THE STRING                                //
////////////////////////////////////////////////////////////
// O.Slavin                                               //
////////////////////////////////////////////////////////////
// Creation     : 20 Jul 1998                             //
////////////////////////////////////////////////////////////
// Modification : 21 Jul 1998                             //
////////////////////////////////////////////////////////////
#ifndef __CSTR_H
   #define __CSTR_H

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

   #include "ccomdefs.h"
   #include "cstrdefs.h"

#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"

   #ifdef __CSTR__
      #define CSTR_FUNC  FUN_EXPO
   #else
      #define CSTR_FUNC  FUN_IMPO
   #endif

   #ifdef __cplusplus
   extern "C" {
   #endif

/////////////////////////////////
// AK 22.02.99
//#ifndef Handle
//typedef void * Handle ;
//#endif
#include "cttypext.h"
///////////////////////////////AK

   CSTR_FUNC(Bool32)  CSTR_Init( Word16 wHeightCode , Handle hStorage);
   CSTR_FUNC(void)    CSTR_Done(void);
   CSTR_FUNC(Word32)  CSTR_GetReturnCode(void);
   CSTR_FUNC(char*)   CSTR_GetReturnString(Word32 dwError);
   CSTR_FUNC(Bool32)  CSTR_SetImportData(Word32 dwType, void * pData);
   CSTR_FUNC(Bool32)  CSTR_GetExportData(Word32 dwType, void * pData);

    enum CSTRFunctionsExt
        {
        CSTR_FNNEWLINE,
        CSTR_FNDELLINE,
        CSTR_FNSETSTRATTR,
        CSTR_FNGETSTRATTR,
        CSTR_FNGETFIRST,
        CSTR_FNGETLAST,
        CSTR_FNGETNEXT,
        CSTR_FNGETPREV,
        CSTR_FNGETLINE,
        CSTR_FNSTORELINE,
        CSTR_FNNEWRASTER,
        CSTR_FNSTORERASTER,
        CSTR_FNSTORECOMP,
        CSTR_FNSTORECOLLECTION,
        CSTR_FNDELRASTER,
        CSTR_FNVERIFY,
        CSTR_FNGETRASTER,
        CSTR_FNGETATTR,
        CSTR_FNGETCOLLECT,
        CSTR_FNSETUSER,
        CSTR_FNGETUSER,
        CSTR_FNSETATTR,
        CSTR_FNCOPYLINE,
        CSTR_FNGETLNHND,
        CSTR_FNGETCOMP,
        CSTR_FNNEWUSERCODE,
        CSTR_FNVERSION,
        CSTR_FNTOTXT,
        CSTR_FNINSRST,
        CSTR_FNMAXNUM,
        CSTR_FNFIRSTLN,
        CSTR_FNNEXTTLN,
        CSTR_FNDELALL,
        CSTR_FNNEXT,
        CSTR_FNPREV,
        CSTR_FNSWAP,
        CSTR_FNDELSAVE,
        CSTR_FNRESTRAST,
        CSTR_FNCOMPOSE,
        CSTR_KILLIM,
        CSTR_FNSAVE,
        CSTR_FNGETLIST,
        CSTR_FNSETLIST,
        CSTR_FNGETCOLLECTIONUNI,
        CSTR_FNSTORECOLLECTIONUNI,
        CSTR_FNGETCONTAINER,
        CSTR_FNSTORESCALE,
        CSTR_FNGETLINEFIRST,
        CSTR_FNGETLINENEXT,
        CSTR_FNCLEARLINE,
        CSTR_FNGETRASTERLINE,
        CSTR_FNSETDUP    ,
        CSTR_FNSETDUPEND ,
        CSTR_FNGETDUP    ,
        CSTR_FNGETDUPEND ,
        CSTR_FNPACK,
        CSTR_FNUNPACK,
        CSTR_FNSORTFRAGM,
        CSTR_FNFIRSTLINEFRAGM,
        CSTR_FNNEXTLINEFRAGM,
        CSTR_FNSTORECOMPORIGINAL,
        CSTR_FNGETMAXFRAGMENT,
        CSTR_SAVECONT,
        CSTR_RESTORECONT,
        CSTR_INSERTRASTERDOWN,
        CSTR_COPYRASTER,
        CSTR_DELETELOOPS,
        CSTR_GETNEXTDOWN,
        CSTR_GETPREVDOWN,
        CSTR_GETNEXTUP,
        CSTR_GETPREVUP,
        CSTR_GETLENGTH,
        CSTR_FNTOTXT_COORD
        };
    enum CSTRFunctionsImp
        {
        CSTR_FNIMP_ALLOC,
        CSTR_FNIMP_REALLOC,
        CSTR_FNIMP_FREE,
        CSTR_FNIMP_OPEN,
        CSTR_FNIMP_CLOSE,
        CSTR_FNIMP_READ,
        CSTR_FNIMP_WRITE
        };

// 1    CSTR_FNNEWLINE      ���������� ����� ������
typedef CSTR_line       (*FNCSTR_NewLine)(Int32  lineno, Int32 version, Int32 container);
CSTR_FUNC(CSTR_line)    CSTR_NewLine (Int32  lineno, Int32 version, Int32 container);
// 2    CSTR_FNDELLINE      ������� ������
typedef Bool32          (*FNCSTR_DeleteLine)(CSTR_line  line);
CSTR_FUNC(Bool32)           CSTR_DeleteLine (CSTR_line  line);
// 3    CSTR_FNSETSTRATTR   ���������� �������� ������� ������
typedef Bool32          (*FNCSTR_SetLineAttr)(CSTR_line  line,CSTR_attr * attr);
CSTR_FUNC(Bool32)               CSTR_SetLineAttr (CSTR_line      line,CSTR_attr * attr);
// 4    CSTR_FNGETSTRATTR   �������� �������� ������� ������
typedef Bool32          (*FNCSTR_GetLineAttr)(CSTR_line  line,CSTR_attr * attr);
CSTR_FUNC(Bool32)           CSTR_GetLineAttr (CSTR_line  line,CSTR_attr * attr);
// 5    CSTR_FNGETFIRST     ���� ����� ������� (����������) ������ � ������
typedef CSTR_rast       (*FNCSTR_GetFirstRaster)(CSTR_line      line);
CSTR_FUNC(CSTR_rast)    CSTR_GetFirstRaster (CSTR_line  line);
// 6    CSTR_FNGETLAST      ���� ����� ���������� (����������) ������ � ������
typedef CSTR_rast       (*FNCSTR_GetLastRaster)(CSTR_line       line);
CSTR_FUNC(CSTR_rast)    CSTR_GetLastRaster (CSTR_line   line);
// 7    CSTR_FNGETNEXT      ���� ����� ���������� ������
typedef CSTR_rast       (*FNCSTR_GetNextRaster)(CSTR_rast  curr_raster, Word32 type_raster);
CSTR_FUNC(CSTR_rast)    CSTR_GetNextRaster (CSTR_rast  curr_raster, Word32 type_raster);
// 8    CSTR_FNGETPREV      ���� ����� ����������� ������
typedef CSTR_rast       (*FNCSTR_GetPrevRaster)(CSTR_rast  curr_raster, Word32 type_raster);
CSTR_FUNC(CSTR_rast)    CSTR_GetPrevRaster (CSTR_rast  curr_raster, Word32 type_raster);
// 9    CSTR_FNGETLINE      ���� ����� ������, ���������� � �������
typedef CSTR_line       (*FNCSTR_GetLine)(CSTR_rast  curr_raster);
CSTR_FUNC(CSTR_line)    CSTR_GetLine (CSTR_rast  curr_raster);
// 10   CSTR_FNSTORELINE    ������� ������ ������
typedef Bool32          (*FNCSTR_StoreLine)(CSTR_rast  curr_raster, CSTR_line line);
CSTR_FUNC(Bool32)               CSTR_StoreLine (CSTR_rast  curr_raster, CSTR_line line);
// 11   CSTR_FNNEWRASTER    ������� ����� ����� � ������� ������
typedef CSTR_rast               (*FNCSTR_NewRaster)(CSTR_line   line ,Int32 col, Int32 row, Int32 w);
CSTR_FUNC(CSTR_rast)            CSTR_NewRaster (CSTR_line       line ,Int32 col, Int32 row, Int32 w);
// 12   CSTR_FNSTORERASTER  ��������� ����� ��� �����������
typedef Bool32          (*FNCSTR_StoreRaster)(CSTR_rast  curr_raster, RecRaster *image);
CSTR_FUNC(Bool32)               CSTR_StoreRaster (CSTR_rast  curr_raster, RecRaster *image);
// 13   CSTR_FNSTORECOMP ��������� ����� ��� ���������� � ������������ ��������������
typedef Bool32          (*FNCSTR_StoreComp)(CSTR_rast  curr_raster, Word8 *lp, Bool32 raster_init, Word8 scale);
CSTR_FUNC(Bool32)               CSTR_StoreComp (CSTR_rast  curr_raster, Word8 *lp, Bool32 raster_init, Word8 scale);
// 14   CSTR_FNSTORECOLLECTION ��������� ���������
typedef Bool32          (*FNCSTR_StoreCollection)(CSTR_rast  curr_raster, RecVersions *collect);
CSTR_FUNC(Bool32)                   CSTR_StoreCollection (CSTR_rast  curr_raster, RecVersions *collect);
// 15   CSTR_FNDELRASTER    ������� �����
typedef CSTR_rast   (*FNCSTR_DelRaster)(CSTR_rast  curr_raster);
CSTR_FUNC(CSTR_rast)    CSTR_DelRaster (CSTR_rast  curr_raster);
// 16   CSTR_FNVERIFY       ������������� ������ ������
typedef Bool32          (*FNCSTR_VerifyLine)(CSTR_line  line);
CSTR_FUNC(Bool32)               CSTR_VerifyLine (CSTR_line      line);
// 17   CSTR_FNGETRASTER    �������� ����������� (����� ��� ���������)
typedef Bool32          (*FNCSTR_GetImage)(CSTR_rast  curr_raster, Word8 *out_res, Word32 type_image);
CSTR_FUNC(Bool32)               CSTR_GetImage (CSTR_rast  curr_raster, Word8 *out_res, Word32 type_image);
// 18   CSTR_FNGETATTR      �������� �������� ������
typedef Bool32          (*FNCSTR_GetAttr)(CSTR_rast  curr_raster, CSTR_rast_attr * attr);
CSTR_FUNC(Bool32)           CSTR_GetAttr (CSTR_rast  curr_raster, CSTR_rast_attr * attr);
// 19   CSTR_FNGETCOLLECT   �������� ���������
typedef Bool32          (*FNCSTR_GetCollection)(CSTR_rast  curr_raster, RecVersions *result_collect);
CSTR_FUNC(Bool32)                   CSTR_GetCollection (CSTR_rast  curr_raster, RecVersions *result_collect);
// 20   CSTR_FNSETUSER      �������� � ����� ���������������� ������
typedef Bool32          (*FNCSTR_SetUserAttr)(CSTR_rast raster, CCOM_USER_BLOCK  *ub);
CSTR_FUNC(Bool32)               CSTR_SetUserAttr (CSTR_rast raster, CCOM_USER_BLOCK  *ub);
// 21   CSTR_FNGETUSER      ��������� �� ������ ���������������� ������
typedef Bool32          (*FNCSTR_GetUserAttr)(CSTR_rast raster, CCOM_USER_BLOCK  *ub);
CSTR_FUNC(Bool32)               CSTR_GetUserAttr (CSTR_rast raster, CCOM_USER_BLOCK  *ub);
// 22   CSTR_FNSETATTR      �������� �������� ������
typedef Bool32          (*FNCSTR_SetAttr)(CSTR_rast  curr_raster, CSTR_rast_attr * attr);
CSTR_FUNC(Bool32)           CSTR_SetAttr (CSTR_rast  curr_raster, CSTR_rast_attr * attr);
// 23   CSTR_FNCOPYLINE     ����������� ���������� ������
typedef Bool32  (*FNCSTR_CopyLine)(CSTR_line    trg,CSTR_line   src);
CSTR_FUNC(Bool32)       CSTR_CopyLine (CSTR_line        trg,CSTR_line   src);
CSTR_FUNC(Bool32)       CSTR_ReplaceWord(CSTR_rast dest_begin, CSTR_rast dest_end, CSTR_rast src_begin, CSTR_rast src_end);

CSTR_FUNC(Bool32)       CSTR_EmptyLine(CSTR_line lin);

CSTR_FUNC(Bool32)       CSTR_SetNewLineNumber (CSTR_line  linel, int new_number);

// 24   CSTR_FNGETLNHND     ���� ����� ������ �� ������ � ������
typedef CSTR_line   (*FNCSTR_GetLineHandle)(Int32 line_no, Int32 version);
CSTR_FUNC(CSTR_line )   CSTR_GetLineHandle (Int32 line_no, Int32 version);
// 25   CSTR_FNGETCOMP      �������� ���������y ������
typedef CCOM_comp *     (*FNCSTR_GetComp)(CSTR_rast  curr_raster);
CSTR_FUNC(CCOM_comp *)      CSTR_GetComp (CSTR_rast  curr_raster);
// 26   CSTR_FNNEWUSERCODE  �������� ��������� ��� ����������������� �����
typedef Int32      (*FNCSTR_NewUserCode) (void);
CSTR_FUNC(Int32)       CSTR_NewUserCode (void);
// 27   CSTR_FNVERSION      ������ ����������
// 28   CSTR_FNTOTXT        �������� ��������� ����� ������
typedef Bool32            (*FNCSTR_LineToTxt)(CSTR_line lin, char *txt);
CSTR_FUNC(Bool32)                 CSTR_LineToTxt (CSTR_line     lin, char *txt);
// 29   CSTR_FNINSRST       �������� ����� ����� ��������
typedef CSTR_rast       (*FNCSTR_InsertRaster)(CSTR_rast        curr_raster);
CSTR_FUNC(CSTR_rast)    CSTR_InsertRaster (CSTR_rast    curr_raster);
// 30   CSTR_FNMAXNUM       ���������� ����� ������ � ����������
typedef Int32  (*FNCSTR_GetMaxNumber)(void);
CSTR_FUNC(Int32)   CSTR_GetMaxNumber (void);
// 31   CSTR_FNFIRSTLN      ������ ������ ��������� ������
typedef CSTR_line (*FNCSTR_FirstLine)( Int32 version);
CSTR_FUNC(CSTR_line)  CSTR_FirstLine ( Int32 version);
// 32   CSTR_FNNEXTTLN      ��������� ������ ��������� ������
typedef CSTR_line (*FNCSTR_NextLine)( CSTR_line start, Int32 version);
CSTR_FUNC(CSTR_line)  CSTR_NextLine ( CSTR_line start,Int32 version);
// 33   CSTR_FNDELALL       ������� ��� ������
typedef void (*FNCSTR_DeleteAll)(void);
CSTR_FUNC(void) CSTR_DeleteAll(void);
// 34   CSTR_FNNEXT         ��������� ����������� �����
typedef CSTR_rast  (*FNCSTR_GetNext)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast)    CSTR_GetNext(CSTR_rast rst);
// 35   CSTR_FNPREV         ���������� ����������� �����
typedef CSTR_rast  (*FNCSTR_GetPrev)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast)    CSTR_GetPrev(CSTR_rast rst);
// 36   CSTR_FNSWAP         �������� ������� 2 ������
typedef Bool32 (*FNCSTR_SwapRasters)(CSTR_rast r1, CSTR_rast r2);
CSTR_FUNC(Bool32) CSTR_SwapRasters(CSTR_rast r1, CSTR_rast r2);
// 37   CSTR_FNDELSAVE      ������� � �����������
typedef CSTR_rast (*FNCSTR_DelSaveRaster)(CSTR_rast c);
CSTR_FUNC(CSTR_rast)    CSTR_DelSaveRaster(CSTR_rast c);
// 38   CSTR_FNRESTRAST     ������������
typedef CSTR_rast (*FNCSTR_RestoreRaster)(CSTR_line ln,CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_RestoreRaster(CSTR_line ln,CSTR_rast rst);
// 39   CSTR_FNCOMPOSE      ������� ������ �������
typedef CSTR_rast (*FNCSTR_compose_Cell)(Int32 n,CSTR_rast *clist, Int32 nIncline, Bool32 NeedDel);
CSTR_FUNC(CSTR_rast) CSTR_compose_Cell(Int32 n,CSTR_rast *clist, Int32 nIncline, Bool32 NeedDel);
// 40   CSTR_KILLIM         ����� ����� � ������
typedef Bool32  (*FNCSTR_KillImage)(CSTR_rast rst);
CSTR_FUNC(Bool32) CSTR_KillImage(CSTR_rast rst);
// 41   CSTR_FNSAVE
typedef CSTR_rast    (*FNCSTR_SaveRaster)(CSTR_rast c);
CSTR_FUNC(CSTR_rast)    CSTR_SaveRaster(CSTR_rast c);
// 42   CSTR_FNGETLIST
typedef CSTR_rast (*FNCSTR_GetComplist)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetComplist(CSTR_rast rst);
// 43   CSTR_FNSETLIST
typedef Bool32 (*FNCSTR_SetComplist)(CSTR_rast rst, CSTR_rast complist);
CSTR_FUNC(Bool32) CSTR_SetComplist(CSTR_rast rst, CSTR_rast complist);
// 44   CSTR_FNGETCOLLECTIONUNI ���� ����������� ���������
typedef Bool32(*FNCSTR_GetCollectionUni) (CSTR_rast  curr_raster, UniVersions *result_collect);
CSTR_FUNC(Bool32)                   CSTR_GetCollectionUni (CSTR_rast  curr_raster, UniVersions *result_collect);
// 45   CSTR_FNSTORECOLLECTIONUNI ��������� ����������� ���������
typedef Bool32(*FNCSTR_StoreCollectionUni) (CSTR_rast  curr_raster, UniVersions *result_collect);
CSTR_FUNC(Bool32)                   CSTR_StoreCollectionUni (CSTR_rast  curr_raster, UniVersions *result_collect);
// 46   CSTR_FNGETCONTAINER ���� ����� ���������� ������
typedef CCOM_handle (*FNCSTR_GetContainer)(CSTR_line ln);
CSTR_FUNC(CCOM_handle) CSTR_GetContainer(CSTR_line ln);
// 47   CSTR_FNSTORESCALE   ��������� ��� ���������������
typedef Bool32 (*FNCSTR_StoreScale)(CSTR_rast  curr_raster,Word8 scale);
CSTR_FUNC(Bool32)               CSTR_StoreScale(CSTR_rast  curr_raster,Word8 scale);
// 48   CSTR_GETLINEFIRST   ���� ����� ������ ������ ������ ��������� � ������
typedef CSTR_line (*FNCSTR_GetLineFirst) ( Int32 fragment_no, Int32 version);
CSTR_FUNC(CSTR_line )   CSTR_GetLineFirst ( Int32 fragment_no, Int32 version);
// 49   CSTR_GETLINENEXT    ���� ����� ��������� ������ ������ ��������� � ������
typedef CSTR_line (*FNCSTR_GetLineNext) (CSTR_line lin, Int32 fragment_no, Int32 version);
CSTR_FUNC(CSTR_line )   CSTR_GetLineNext (CSTR_line lin, Int32 fragment_no, Int32 version);
// 50   CSTR_FNCLEARLINE    ������� ������ �� ������
typedef Bool32 (*FNCSTR_ClearLine)(CSTR_line lin,Int16 left, Int16 right);
CSTR_FUNC(Bool32) CSTR_ClearLine(CSTR_line lin,Int16 left, Int16 right);
// 51   CSTR_GETRASTERLINE      ���� ����� ������ ������
typedef CSTR_line  (*FNCSTR_GetRasterLine) (CSTR_rast  curr_raster);
CSTR_FUNC(CSTR_line)    CSTR_GetRasterLine (CSTR_rast  curr_raster);
// 52   CSTR_SETDUP             ��������� ����� ����������� ������
typedef Bool32 (*FNCSTR_SetDup)(CSTR_rast   rus, CSTR_rast   eng);
CSTR_FUNC(Bool32) CSTR_SetDup(CSTR_rast   rus, CSTR_rast   eng);
// 53   CSTR_SETDUPEND          ��������� ����� ��������������� ����������� ������
typedef Bool32 (*FNCSTR_SetDupEnd)(CSTR_rast   rus,CSTR_rast    eng);
CSTR_FUNC(Bool32) CSTR_SetDupEnd(CSTR_rast   rus,CSTR_rast    eng);
// 54   CSTR_GETDUP             ���� ����� ����������� ������
typedef CSTR_rast (*FNCSTR_GetDup)(CSTR_rast rus);
CSTR_FUNC(CSTR_rast) CSTR_GetDup(CSTR_rast rus);
// 55   CSTR_GETDUPEND          ���� ����� ��������������� ����������� ������
typedef CSTR_rast (*FNCSTR_GetDupEnd)(CSTR_rast rus);
CSTR_FUNC(CSTR_rast) CSTR_GetDupEnd(CSTR_rast rus);
// 56   CSTR_FNPACK:            ������� ����������� ����������
typedef Bool32 (*FNCSTR_PackLine)(CSTR_line line);
CSTR_FUNC(Bool32) CSTR_PackLine(CSTR_line line);
// 57   CSTR_FNUNPACK:          ������������ ����������� ����������
typedef Bool32 (*FNCSTR_UnpackLine)(CSTR_line line);
CSTR_FUNC(Bool32) CSTR_UnpackLine(CSTR_line line);
// 58   CSTR_FNSORTFRAGM        ������������� �� ����������
typedef Bool32  (*FNCSTR_SortFragm)( Int32 version);
CSTR_FUNC(Bool32)       CSTR_SortFragm ( Int32 version);
// 59   CSTR_FNFIRSTLINEFRAGM   ������ �� ���������
typedef CSTR_line       (*FNCSTR_FirstLineFragm)( Int32 fragm, Int32 version );
CSTR_FUNC(CSTR_line)    CSTR_FirstLineFragm ( Int32 fragm, Int32 version );
// 60   CSTR_FNNEXTLINEFRAGM    ��������� �� ���������
typedef CSTR_line       (*FNCSTR_NextLineFragm)( CSTR_line start);
CSTR_FUNC(CSTR_line)    CSTR_NextLineFragm ( CSTR_line start);
// 61   CSTR_FNSTORECOMPORIGINAL    ��������� ���������� (��� ������������� ����������)
typedef Bool32          (*FNCSTR_StoreCompOriginal) (CSTR_rast  curr_raster, CCOM_comp   * comp, Bool32 raster_init);
CSTR_FUNC(Bool32)               CSTR_StoreCompOriginal (CSTR_rast  curr_raster, CCOM_comp   * comp, Bool32 raster_init);
// 62   CSTR_FNGETMAXFRAGMENT   ������������ ����� ���������
typedef Int32    (*FNCSTR_GetMaxFragment)(Int32 version);
CSTR_FUNC(Int32)    CSTR_GetMaxFragment(Int32 version);
// 63   CSTR_SAVECONT           ��������� ��������� �� �����
typedef Bool32 (*FNCSTR_SaveCont)(char *filename);
CSTR_FUNC(Bool32) CSTR_SaveCont(char *filename);
// 64   CSTR_RESTORECONT        ������������ ��������� � �����
typedef Bool32 (*FNCSTR_RestoreCont)( char *filename);
CSTR_FUNC(Bool32) CSTR_RestoreCont(char *filename);
// 65   CSTR_INSERTRASTERDOWN   �������� ������ ����
typedef Bool32  (*FNCSTR_InsertRasterDown) (CSTR_rast   start_raster, CSTR_rast stop_raster);
CSTR_FUNC(CSTR_rast)            CSTR_InsertRasterDown (CSTR_rast        start_raster, CSTR_rast stop_raster);
// 66   CSTR_COPYRASTER                 ����������� ���������� ������
typedef Bool32  (*FNCSTR_CopyRaster)(CSTR_rast  trg,CSTR_rast   src);
CSTR_FUNC(Bool32)       CSTR_CopyRaster(CSTR_rast       trg,CSTR_rast   src);
// 67   CSTR_DELETELOOPS                ������� �������� ����
typedef Bool32              (*FNCSTR_DeleteLoops) (CSTR_line    lin);
CSTR_FUNC(Bool32)                   CSTR_DeleteLoops (CSTR_line lin);
// 68   CSTR_GETNEXTDOWN                ������ �� ����� �����
typedef CSTR_rast    (*FNCSTR_GetNextDown)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast)    CSTR_GetNextDown(CSTR_rast rst);
// 69   CSTR_GETPREVDOWN                ������ �� ����� �����
typedef CSTR_rast    (*FNCSTR_GetPrevDown)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast)    CSTR_GetPrevDown(CSTR_rast rst);
// 70   CSTR_GETNEXTUP                  ������ �� ����� �����
typedef CSTR_rast    (*FNCSTR_GetNextUp)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast)    CSTR_GetNextUp(CSTR_rast rst);
// 71   CSTR_GETPREVUP                  ������ �� ����� �����
typedef CSTR_rast    (*FNCSTR_GetPrevUp)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast)    CSTR_GetPrevUp(CSTR_rast rst);
// 72   CSTR_GETLENGTH                  ����� ������� ���������� ������
typedef Int32        (*FNCSTR_GetLength) (CSTR_line   lin);
CSTR_FUNC(Int32)        CSTR_GetLength (CSTR_line   lin);
// 73   CSTR_FNTOTXT_COORD              �������� ��������� ����� ������ � ������������
typedef Bool32       (*FNCSTR_LineToTxt_Coord)(CSTR_line lin, char *txt, Int32 len);
CSTR_FUNC(Bool32)       CSTR_LineToTxt_Coord (CSTR_line     lin, char *txt, Int32 len);


   #ifdef __cplusplus
            }
   #endif

#endif
