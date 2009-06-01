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
// This file cration date: 10.18.98
//
// frmtfile.h
//
// ============================================================================

#ifndef __frmtfile_h__
#define __frmtfile_h__
#include <stdio.h>
#include "cstrdefs.h"

BOOL CreateInternalFileForFormatter(FILE *fpInternalFileName);
BOOL CheckComingLine( CSTR_line* Comingline );
BOOL CheckLineForFilling( CSTR_line* Comingline );

//BOOL CheckRect(InternalRect* Inner);

/////////////////////////////////////////////////////////////////////////////

class CFragment;
class CFString;
class CWord;
class CChar;

class CRtfString;

struct PageElementCount
{
 WORD  Frags;
 WORD  Strings;
 WORD  Words;
 WORD  Chars;
};

typedef struct tagInternalRect {
 Int16 left;
 Int16 top;
 Int16 right;
 Int16 bottom;
 } InternalRect;


void VCopyRect(InternalRect* InnerR, RECT* OuterR);
BOOL CheckRect(InternalRect* Inner);


/////////////////////////////////////////////////////////////////////////////
// Page class

class CFPage //: public CObject
{
public:
  CFPage();
 ~CFPage();

  CFragment*       GetFirstFrag();
  CFragment*       GetNextFrag();
  void             ProcessingComingLine( CSTR_line* line );
		void             CreateArray_For_TextFragments( );
  BOOL             CheckComingLine_For_TextFragments( CSTR_line* line );
	 void             AddString( CSTR_line* line );
  BOOL	            Write();

  /*CDWordArray*/std::vector<DWORD>      FragmentsArray;
		PageElementCount Count;
	 WORD             m_wDpi;
	 std::vector<CFragment*>/*CObArray*/	        m_arFrags;
  int		            m_nIndex;
  int              m_nCurFragNumber;
  int              m_nPrevFragNumber;
};


/////////////////////////////////////////////////////////////////////////////
// Fragment class

class CFragment//:public CObject
{
public:
  CFragment();
 ~CFragment();

  CFString*	 GetFirstString();
  CFString*	 GetNextString();
  void       AddString( CSTR_line* line , PageElementCount* Count );
  BOOL	     Write();

  WORD     	 m_wStringsCount;
  /*CObArray*/ std::vector<CFString*>	 m_arStrings;
  WORD	     m_wIndex;
  RECT	     m_rectFrag;
  WORD	     m_wType;
  DWORD      m_wUserNumber;
  DWORD      m_Flags;
};

/////////////////////////////////////////////////////////////////////////////
// String class

class CFString//:public CObject
{
public:
  CFString();
 ~CFString();

  CWord*          GetFirstWord();
  CWord*          GetNextWord();
  void			  ExtractWordsFromString( CSTR_line* line ,  PageElementCount* Count );
  void            ExtractNextWord( CSTR_line* line );
  BOOL	          Write();

  WORD  	      m_wWordsCount;
  /*CObArray*/ std::vector<CWord*>        m_arWords;
  WORD	          m_wIndex;
  CFragment*      m_Frag;
  RECT	          m_rectString;
  WORD	          m_wType;
  RECT		      m_rectBaseLine;
  Word32          S_Flags; //NEGA_STR vmk 10-06-2001
};

/////////////////////////////////////////////////////////////////////////////
// Word class

class CWord//:public CObject
{
public:
  CWord();
 ~CWord();

  CChar*          GetFirstChar();
  CChar*          GetNextChar();
  void            AddLetter2Word( CSTR_rast* rast , PageElementCount* Count ,BOOL* FlagCapDrop);
  BOOL	           Write();

  /*CObArray*/std::vector<CChar*>        m_arChars;
  WORD	           m_wCharsCount;
  WORD	           m_wIndex;
  CFString*       m_String;
  RECT	          m_rectWord;
  WORD	           m_wFontNumber;
  WORD	           m_wFontPointSize;
};

/////////////////////////////////////////////////////////////////////////////
// Char class

class CChar//:public CObject
{
public:
	CChar();
	~CChar();
	void         AddingLetter( CSTR_rast* rast , int index ,BOOL* FlagCapDrop);
	BOOL         Write();

	RECT	       m_rectChar;
	RECT	       m_RealRectChar;
	struct
	{
		BYTE	   m_bChar;
		BYTE	   m_bProbability;
	} m_chrVersions[REC_MAX_VERS];//!!! Art
	BYTE         m_blanguage;
	BYTE         m_bFlg_spell;
	BYTE         m_bFlg_spell_nocarrying;
	BYTE         m_bFlg_cup_drop;
	WORD         m_wCountAlt;
	WORD         m_wFontNumber;
	WORD         m_wIndex;
 };

/////////////////////////////////////////////////////////////////////////////

#endif // __frmtfile_h__
