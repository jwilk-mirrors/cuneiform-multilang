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

#ifndef __RSTR_H
   #define __RSTR_H

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

#ifdef __RSTR__
	#define RSTR_CLASS CLA_EXPO
#else
	#define RSTR_CLASS CLA_IMPO
#endif

   #ifndef __MEMFUNC_H
   #include "memfunc.h"
   #endif

   #ifndef __CSTR_H
   #include "cstr.h"
   #endif

   #ifndef __SNPDEFS_H
   #include "snpdefs.h"
   #endif
/*
   #ifndef __FRHDEFS_H
   #include "frhdefs.h"
   #endif
*/
//   #ifndef __RSTRDEFS_H
//   #include "rcmdefs.h"
   #include "rstrdefs.h"
  // #endif

   #ifdef __RSTR__    // ReCog Main
      #define RSTR_FUNC  FUN_EXPO
   #else
      #define RSTR_FUNC  FUN_IMPO
   #endif

#ifndef __cplusplus
   typedef struct tagIRstrDelta
   {
      Rect16    dr;        // bounds delta
      Point16   dc;        // centers delta
      Point16   dhw;       // height-width delta
   }  IRstrDelta;
	_SETTYPES(IRstrDelta)
#else
   class RSTR_CLASS IRstrDelta
   {
      public:
      Rect16    dr;        // bounds delta
      Point16   dc;        // centers delta
      Point16   dhw;       // height-width delta
      IRstrDelta( const Rect16 & r, const Point16 & p ): dr(r), dc(p), dhw(p) {};
      IRstrDelta( const Rect16 & r, const Point16 & p, const Point16 & hw): dr(r), dc(p), dhw(hw) {};
      void     Set( const Rect16 & r, const Point16 & p ){ dr = r; dc = p; dhw = p; };
      void     Set( const Rect16 & r, const Point16 & p, const Point16 & hw ){ dr = r; dc = p; dhw = hw; };
      IRstrDelta(){};
   };
	_SETTYPES(IRstrDelta)
	_SETCLASS(IRstrDelta)
#endif // __cplusplus

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct tagRSTR_info{
    Int32   ndig, nfdig, nall, alphabet, ndollars, nplus_minus;
    } RSTR_info;

   RSTR_FUNC(Bool32)  RSTRInit( MemFunc* mem );

   RSTR_FUNC(Bool32)  RSTRNewPage( Int32 resolutiony, Handle myPage );
//   RSTR_FUNC(Bool32)  RSTRNewPage( FrhPageSetup* setup, Handle myPage );

   RSTR_FUNC(void)    RSTRRegisterSnpTree(
            SnpTreeNode* parent,         // parent Snp Node, may be NULL
            __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                                 );
/*
   RSTR_FUNC(Bool32)  RSTRSetup(
      const FrhFieldSetup  setup,
          Int32 pageSkew
                             );
*/
   RSTR_FUNC(void)  RSTRDone(void);

   RSTR_FUNC(Err16) RSTRGetErr(void);
// OLEG for PUMA
   RSTR_FUNC(Bool32)  RSTRSnapInit( void );
/////////////////////////////////
RSTR_FUNC(Bool32)  RSTR_RecogContainer (void);

#include "cttypext.h"
///////////////////////////////AK

RSTR_FUNC(Bool32)  RSTR_Init(Word16 wHightCode, Handle hStorage);
RSTR_FUNC(Bool32)  RSTR_Done(void);
RSTR_FUNC(Word32)  RSTR_GetReturnCode(void);
RSTR_FUNC(Word8 *) RSTR_GetReturnString(Word32 dwError);
RSTR_FUNC(Bool32)  RSTR_GetExportData (Word32 dwType, void * pData);
RSTR_FUNC(Bool32)  RSTR_SetImportData (Word32 dwType, const void * pData);
///
enum RSTRFunctions
    {
    RSTR_FNNEWPAGE,
    RSTR_FNSETOPTIONS,
    RSTR_FNBITMAP,
    RSTR_FNLINE,
    RSTR_FNVERSION,
    REXC_FNRSTR_ISLANGUAGE,
    RSTR_FNIMP_FREE,
    RSTR_FNIMP_ALLOC,
    RSTR_Word8_Language,
    RSTR_Word16_Resolution,
    RSTR_Word8_Fax1x2,
    RSTR_Word8_Matrix,
    RSTR_Word8_P2_active,
    RSTR_Word8_Spell_check,
    RSTR_pchar_user_dict_name,
    RSTR_FNSTRENDPAGE,
    RSTR_FNNEEDPASS2,
    RSTR_FNIMP_GETCOLORS,
    RSTR_pchar_temp_dir,
    RSTR_OcrPath,
    RSTR_Word8_spec_camera,
    RSTR_Word8_spec_nolinpen,
    RSTR_Word8_mmx,
    RSTR_FNLINEBL,
    RSTR_FNTESTALPHABET,
    RSTR_RECONEWORD,
    RSTR_FNSTUDYALPHABET,
    RSTR_FNRECOGONELETTER,
    RSTR_FNRECOGCONTAINER,
    RSTR_Word8_P2_disable,
    RSTR_FNRECOGONELETTERALL,
    RSTR_FNTESTALPHABETPASS2,
    RSTR_FNSTUDYALPHABETPASS2,
    RSTR_FNCHANGELINENUMBER,
	RSTR_SETSPECPRJ,
	RSTR_CTB_BASE_NAME,
	RSTR_CTB_GRAY_NAME
    };

// 1)   RSTR_FNNEWPAGE      ����� ��������
typedef Bool32(*FNRSTR_NewPage)(Int32 resolutiony, Handle myPage);
RSTR_FUNC(Bool32)  RSTR_NewPage (Int32 resolutiony, Handle myPage);
//typedef Bool32(*FNRSTR_NewPage)(FrhPageSetup* setup, Handle myPage);
//RSTR_FUNC(Bool32)  RSTR_NewPage (FrhPageSetup* setup, Handle myPage);
// 2)   RSTR_FNSETOPTIONS   ��������� �����
typedef Bool32(*FNRSTR_SetOptions)(RSTR_Options *opt);
RSTR_FUNC(Bool32)  RSTR_SetOptions (RSTR_Options *opt);
// 3)   RSTR_FNBITMAP       ���������� �� ����
typedef Bool32(*FNRSTR_RecogBitmap)(Rect16 *r, Handle bm,CSTR_line lino);
RSTR_FUNC(Bool32)  RSTR_RecogBitmap (Rect16 *r, Handle bm,CSTR_line lino);
// 4)   RSTR_FNLINE         ����������
typedef Bool32(*FNRSTR_Recog)(CSTR_line lini, CSTR_line lino);
RSTR_FUNC(Bool32)  RSTR_Recog (CSTR_line lini, CSTR_line lino);
// 5)   RSTR_FNVERSION      ������ ����������
//
// 6)   REXC_FNRSTR_ISLANGUAGE �������� �� ����� ����
typedef Bool32(*FNRSTR_IsLanguage)(Word8 language);
RSTR_FUNC(Bool32) RSTR_IsLanguage(Word8 language);
// 7    RSTR_FNSTRENDPAGE    �������� ����� ����� ��������� ������
typedef Bool32 (*FNRSTR_EndPage)(  Handle myPage );
RSTR_FUNC(Bool32)  RSTR_EndPage(  Handle myPage );
// 8    RSTR_FNNEEDPASS2        �������� �� ������ ������
typedef Bool32 (*FNRSTR_NeedPass2)( void );
RSTR_FUNC(Bool32)  RSTR_NeedPass2( void );
// 9)   RSTR_FNLINEBL       ���������� ��
typedef Bool32(*FNRSTR_RecogBL) (CSTR_line lini);
RSTR_FUNC(Bool32)  RSTR_RecogBL (CSTR_line lini);
// 10)  RSTR_FNTESTALPHABET ���� ���������� ���������� � �������
typedef Bool32 (*FNRSTR_TestAlphabet)(RSTR_info *str,CSTR_line lin);
RSTR_FUNC(Bool32) RSTR_TestAlphabet(RSTR_info *str,CSTR_line lin);
// 11   RSTR_RECONEWORD     ���������� ������������� �����
typedef Bool32 (*FNRSTR_recog_one_word)(CSTR_line ln, Word8 *word, Int8 *points,Word8 *res);
RSTR_FUNC(Bool32) RSTR_recog_one_word(CSTR_line ln, Word8 *word, Int8 *points,Word8 *res);
// ������ ������� ����� - ��� ��������� ������, ��������� - ��������������� ������ ���� (��������� � ����),
// ��������� - ������ ���������� � points ����
// 12   RSTR_FNSTUDYALPHABET    ����� �������� ������� �� ����������
typedef Bool32 (*FNRSTR_StudyAlphabet)(RSTR_info *str);
RSTR_FUNC(Bool32) RSTR_StudyAlphabet(RSTR_info *str);
// 13   RSTR_FNRECOGONELETTER   ���������� ��� ������� ���������� ��� ������� �����
typedef Bool32  (*FNRSTR_RecogOneLetter) (RecRaster *Rs,Word8 Language,RecVersions *Vs);
RSTR_FUNC(Bool32)  RSTR_RecogOneLetter (RecRaster *Rs,Word8 Language,RecVersions *Vs);
// 14   RSTR_FNRECOGCONTAINER   ���������� ��������� �����
typedef Bool32  (*FNRSTR_RecogContainer) (void);
RSTR_FUNC(Bool32)  RSTR_RecogContainer (void);
// 15   RSTR_FNRECOGONELETTERALL   ���������� ��� ����� �� ��������
typedef Bool32  (*FNRSTR_RecogOneLetter_all) (RecRaster *Rs,char *letters,RecVersions *Vs,Int32 nType);
RSTR_FUNC(Bool32)  RSTR_RecogOneLetter_all (RecRaster *Rs,char *letters,RecVersions *Vs,Int32 nType);
// 16   RSTR_FNTESTALPHABETPASS2 ���� ���������� ���������� � ������� ����� ������ ��������
typedef Bool32 (*FNRSTR_TestAlphabetPass2)(RSTR_info *str,CSTR_line lin);
RSTR_FUNC(Bool32) RSTR_TestAlphabetPass2(RSTR_info *str,CSTR_line lin);
// 17   RSTR_FNSTUDYALPHABETPASS2    ����� �������� ������� �� ���������� ����� ������ ��������
typedef Bool32 (*FNRSTR_StudyAlphabetPass2)(RSTR_info *str);
RSTR_FUNC(Bool32) RSTR_StudyAlphabetPass2(RSTR_info *str);
// 18   RSTR_FNCHANGELINENUMBER   �������� ����� ������� ������
typedef Bool32 (*FNRSTR_ChangeLineNumber)(Int32 add);
RSTR_FUNC(Bool32) RSTR_ChangeLineNumber(Int32 add);
// 19   RSTR_SETSPECPRJ   ���������� ������� ����-�������
typedef Bool32 (*FNRSTR_SetSpecPrj)(Word8 nSpecPrj);
RSTR_FUNC(Bool32) RSTR_SetSpecPrj(Word8 nSpecPrj);

RSTR_FUNC(Word8 *) RSTR_GetLingPath();

//���������� �� ����� ������ ������
RSTR_FUNC(Bool32)  RSTRRecognizeMain(CSTR_line lin, CSTR_line lino);
RSTR_FUNC(void) RSTR_Save2CTB(CSTR_line lino,Int32 type, Int16 line_num);
RSTR_FUNC(Bool32)  RSTRRecognizePostMain(CSTR_line lin, CSTR_line lino);

#ifdef __cplusplus
            }
#endif

#ifndef __cplusplus
   typedef struct tagIRstr
   {
      Rect16   rmn;
      Rect16   rmx;
      Point16  cmn;
      Point16  cmx;
      Point16  hwmn;
      Point16  hwmx;
   }  IRstr;

#else
   _SETCLASS(IRstr)
   class RSTR_CLASS IRstr
   {
      public:
      Rect16    rmn;
      Rect16    rmx;
      Point16   cmn;
      Point16   cmx;
      Point16   hwmn;
      Point16   hwmx;
      IRstr( const Rect16 & rc ){ Set(rc); };
      void     Set( const Rect16 & rc );
      IRstr(){};
      IRstr( RCIRstr rst, RCIRstrDelta rd ){ Set(rst, rd); }; // extended restriction
      void     Set( RCIRstr rst, RCIRstrDelta rd ); // extended restriction
      Bool Check( const Rect16 & rc ) const;
      Bool Check( const Rect16 & rc, int delta ) const;

      void
      Update( const Rect16 & rc, /*FrmLMask*/ long this2rc );

      void
      Update( RCIRstr rst, /*FrmLMask*/ long this2rc );


      void
      UpdateC();

      void
      UpdateHW();

      void
      UpdateLL( const Rect16 & rc, /*FrmLMask*/ long this2rc, int delta=0 );  // line by line

      void
      SqueezeWidth( int max_width );

      void
      SqueezeHeight( int max_height );

      /** Self checking **/
      Bool     LRGood() const
         {
           return (rmx.left  >= rmn.left  ) &&
                  (rmx.right >= rmn.right ) &&
                  (rmx.right >= rmn.left  );
         };
      Bool     TBGood() const
         {
           return (rmx.top    >= rmn.top     ) &&
                  (rmx.bottom >= rmn.bottom  ) &&
                  (rmx.bottom >= rmn.top     );
         };
      Bool     XYGood() const
         {
            return (cmx.x >= cmn.x) &&
                   (cmx.y >= cmn.y);
         };
      Bool     Good()  const
         {
            return LRGood() && TBGood() && XYGood();
         };

      Bool     LRGood(int Delta) const
         {
           return (rmx.left  +Delta>= rmn.left  ) &&
                  (rmx.right +Delta>= rmn.right ) &&
                  (rmx.right +Delta>= rmn.left  );
         };
      Bool     TBGood(int Delta) const
         {
           return (rmx.top    +Delta>= rmn.top     ) &&
                  (rmx.bottom +Delta>= rmn.bottom  ) &&
                  (rmx.bottom +Delta>= rmn.top     );
         };
      Bool     XYGood(int Delta) const
         {
            return (cmx.x +Delta>= cmn.x) &&
                   (cmx.y +Delta>= cmn.y);
         };
      Bool     Good(int Delta)  const
         {
            return LRGood(Delta) && TBGood(Delta) && XYGood(Delta);
         };

      Bool     operator !() const
         {
            return Good();
         };
   };
#endif // __cplusplus
_SETTYPES(IRstr)
#endif
