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

#ifdef __cplusplus
extern "C" {
#endif

#include "wind32.h"
#include "recdefs.h"

//typedef unsigned char BYTE;


#define SUB_ZN
#define SUB_FRM
	#ifdef WIN_MOD
	    #define FUL
	#endif
//#define MAX_ALT 4
#define DELTA_TITLE 4
//#define CMP
//------�������� ���� TITLE_WORD.W_GEN.W_Spell-------
// SPELL_NOT - ����� �� ��������� ����� SPELL (���� ������ �� ���� ���������
//   � SPELL_CHECK, ���� ����� �����������);
// SPELL_FIND - ����� ���� ��� ����������� �� �������� ���������������� �������;
// SPELL_NOFIND - ����� ���� ��� ����������� �� �������� ���������������� �� �������;
// SPELL_CORR - � �������� ������� ������� ������ ������� ��������� � �����
// SPELL_PART - ������ ����� ���� ����� �����
//     (���� �� ���� ��������, ���� �� ���� ������� ����)
#define SPELL_NOT  0
#define SPELL_FIND 1
#define SPELL_CORR 2
#define SPELL_PART 3
#define SPELL_NOFIND 4
//---��������� ������ (����� ���� ����� ���������� ���������)---
//#define CYR 1
//#define UKR 2
//#define ENG 4

#define FIRST 0
#define NEXT 1

#define MAX_SYM_WORD 100
#define PAR_FUL struct h_par_ful

#define PROC_SPELL 15
#define PROC_OPEN  14

#define MAX_VALUE 10

#pragma pack(1)

//#define ID_SYM struct h_id_sym /*������������� �������*/
#define SRECT struct h_srect
#define ZN struct h_zn
#define TITLE_ZN struct h_title_zn
#define ALT_SPELL struct h_alt_spell
#define TITLE_WORD struct h_title_word
#define TITLE_STR struct h_title_str
#define TITLE_FUL struct h_title_ful
#define W_GEN struct h_w_gen
#define ALT_ZN struct h_alt_zn
#define S_GEN struct h_s_gen
#define RECOG_MODES struct h_recog_modes
#define COOR_COMP struct h_coor_comp
//ID_SYM { unsigned char col,str,comp,word; };
SRECT {int left,top,right,bottom;};
#define LEN_START 3
COOR_COMP
{
	BYTE  start_pos[LEN_START];
	BYTE  buf;
	short lenght;
};
TITLE_ZN { BYTE Z_Code; // identification code header (= 0)
           ID_SYM Z_Id; // identification code
           BYTE Z_Gen;  //����������� ����������
           SRECT Z_Rect;// Frame
           SRECT Z_RealRect;// Real frame
           BYTE Z_Num_Alt;// # of alternatives
           BYTE Z_NumComp;
           #ifdef CMP
            COOR_COMP CoorComp;// Address components (component)
           #endif
         };
ALT_ZN { BYTE a_Code; //��� �������
         BYTE a_Base; //����� ����
         BYTE a_Prob;
         BYTE a_language;
         BYTE a_SpellNoCarrying;
         BYTE a_FlagCupDrop;
         BYTE a_Spell;
         float a_Dist;//������ �������������
       };
ZN { TITLE_ZN Title;     //��������� ����������
     ALT_ZN Alt[REC_MAX_VERS];//������������
   };

W_GEN { WORD W_NumSym; //����� ��������
        BYTE W_Spell;  //Spell-Check
        BYTE Reserv[2];
		WORD FontNumber;
		WORD FontSize;
      };
ALT_SPELL { BYTE Len; //����� ��������� � ����������� \0 (���� - � 1)
            BYTE Reserv;
            int  Penalty;//����� � ������� SPELL �� ������ ������������
            char *Alt; //���� ������������ - ������ � ������� �
          };

TITLE_WORD { BYTE Z_Code; //��������������� ��� ��������� (=1)
             W_GEN W_Gen; //����������� �����
             //SRECT W_Rect;//�����
             int NumAltSpell; //����� ���������
             int PosFirstCorr;//������� ������ ������ ���� Ins,Del
             ALT_SPELL *AltSpell;//���������
             WORD W_Attr; //�������� �����
             #ifndef ID4
              BYTE Dummy[DELTA_TITLE];
             #endif
           };

S_GEN {
			WORD S_NumWord;//����� ���� � ������
		    BYTE HeadLine;
			BYTE Reserv[2];
      };

TITLE_STR {
			BYTE     Z_Code;       //��������������� ��� ��������� (=2)
            S_GEN    S_Gen;        //����������� ������
            SRECT    S_Rect;       //Base Line
			SRECT    S_Real_Rect;  //���������� ������
            WORD     S_Attr;       //�������� ������
			Word32   S_Flags;      //NEGA_STR vmk 10-06-2001
            #ifndef ID4
				BYTE Dummy[DELTA_TITLE];
            #endif
          };
RECOG_MODES
{
	BYTE s[10];
};

//���-�� �������� ����� ������ ������� � �����
typedef struct h_addrXX
{
	short ind,lev;
} ADDR; //������.�����=N�� ������+N������

typedef struct h_lev
{
	SRECT bnd; WORD SpecInt;
	int kp;
	ADDR *Addr;/*[kp+1],���� kp>=0*/
} LEV;

TITLE_FUL
{ WORD wFHSize; //������ ��������� �����
  WORD wZHSize; //������ �����.��������� ����������, ����� ��� ������
  DWORD dNumZ;  //���������� ��������� � �����
  char StrLogo[16];
  char StrVer[2];//����� ������ ��� �����
  RECOG_MODES Recog_Modes;//�������������� �������� �������������
  DWORD FntNameOffset;//�������� ������ ���� ��� � �����
  DWORD ColOffset;//�������� ������ ������� � �����
  DWORD ZOffset;  //�������� ������ ��������� � �����
  BYTE ProcSpell;//������� ��������� Spell
  BYTE Reserv_1;
  WORD nWord;    // ����� ����
  WORD nStr;     // ����� �����
  BYTE Reserv[26];
  //BYTE *Reserv;//malloc(size=wFHSize-46)
};
#pragma pack(2)
#define PAR_FUL struct h_par_ful
PAR_FUL
{ char KodNoRecog;//��� �����������. ������� ��� b_find
  char KodNoRecogOut;//��� �����������. ������� ��� ��������� ������
  int  NumAlt; //����.����� ������� ��������� ��� SPELL
  int IndexDelta; //������ ������ ������� ������� ������� �� ������
  float RelKrit;//�������� ��������� ��������� �������
  float RelDistAltOCR;//�������� ��������� ��������� OCR-Alt
  // 0 - ����������� SPELL-������
  // 1 - ��������� ������� Spell-OCR
  // 2 - ������������ ������� Spell-OCR
  int AllowSpell;
  //��������� ���������� �� ������������
  int AllowCorr; //���������� ��������� ����� �� ��������� ������������
  int AllowCorrShift; //���������� ��������� Shift
  int AllowCorrChifLet; //���������� ��������� �/�
  int AllowCorrRusLat; //���������� ��������� �� �����
  int AllowCorrFnt; //���������� ��������� �� ������
  float DelStick; //�������� ������������ �� ����� ������
  int AllowTestChifCl;//���������� ������������� ����.Alt ��� ����� �/�
  int AllowTestLangCl;//���������� ������������� ����.Alt ��� ����� ���/���
  int AllowCorrChifCl;//���������� ������������� ����.Alt ��� Corr  �/�
  int AllowCorrLangCl;//���������� ������������� ����.Alt ��� Corr  ���/���
  DWORD IntervalTimer;
  //���� �� ������������
  int  ThresholdDist;//����� � ������� ������� ��� SPELL(������ ������ � ����)
  float k1; //Weight NoCoincide Char
  float k2; //Weight NoRecog Char
  float gam_Ins;
  float gam_Del;
};

//---��������� �������
//int OpenFullOut(char *FileName);
//int FormatStr(int nc,SRECT *Bnd,char ***Str,int **Ksym,int ncOut);
BYTE KoderSym(BYTE Base,BYTE Kod);
int PASC GenFullTxt(char *FileNameFul,char *FileNameOut);
int AnalysLocationPairRect(SRECT b1,SRECT b2);
int BelongPointRect(SRECT b1,int x,int y);
int CalcMoveSep(SRECT b1,SRECT b2,int *dx1,int *dy1);
int OrderBndTxt(SRECT *BndColTxt);
void MoveOneCol(SRECT *Bnd,int dx,int dy);
int FreeStructFull(void);
int RtfFreeStructFull(void);
//int SaveFull(char *FileName);
void init1_par_ful(void);
//int ChooseBestWord(ZN *Zn,TITLE_WORD t,char *word,float k1,float k2,
//    float gam_Ins,float gam_Del);
//int ChooseRelationMin(TITLE_WORD t);
float FindPosDel(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int *pos);
float FindPosIns(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int *pos);
float DistDel(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int pos);
float DistIns(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int pos);
float DistEqNum(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max);
float DistAltZn(ZN x,char y,float k1,float k2,float D_Max);
//float DistMaxWord(ZN *Zn,int kz);
//float Dist2MaxWord(ZN *Zn,int kz);
//--FullSpellFull--
//int GenFullSpellFull(char *FileNameFul,char *FileNameOut,int CodLang,
//    char *NameCorr);
int PASC GenFullSpellFull(char *FileNameFul,char *FileNameOut,INF_TREE *Inf);
int GetNextAltWord(ZN *WordAlt,int k,int Reg,int CodLang,uchar *word,float *dist);
int TstCarry(int nc,int ns,int nw);
//int TestPunctuationMarks(char sym);
//int init_ful();
int AllowWordLang(uchar *word,int CodLang);
int TstCarry(int nc,int ns,int nw);
int PutStructAltSpell(BYTE *buf,WORD  *Dist,TITLE_WORD *t);
int InsertWord(int nc,int ns,int nw,ZN *zi,int kz);
//int TstNameOwr(uchar LastCod,uchar FirstCod);
//uchar GetAlt(int nc,int ns,int nw,int nz,int na);
//int PASC InitSpell(char *PathName,int cod,int TypeDoc1);
#ifndef WIN_MOD
//void AnsiToOem(const char *word1,char *word2);
//void OemToAnsi(const char *word1,char *word2);
#endif
//WORD Penalty1LenWord(int n);
int NormParSpellOCR(ZN *z,int kz,int flo,WORD *reinst,WORD *reinstb,WORD *redel,
    WORD *reelse,WORD *reunknow,float *del,WORD *porog);
int Norm2ParSpellOCR(ZN *z,int kz,int flo,WORD *reinst,WORD *reinstb,WORD *redel,
    WORD *reelse,WORD *reunknow,float *del,WORD *porog);
int FindWord(int flo,ZN *z,BYTE *word,BYTE *buf,WORD *DistMin,WORD *porog1,
TITLE_WORD *t,WORD MaxAltSpell);
//void SetTimerOwn(DWORD NumMs);
//int PASCAL _export killOwn(void);
//--SpelOneWord--
int CloseSpell(void);
int PASC SpellOneWord(uchar *in,uchar *out,int MaxAltSpell);
int PASC CorrOneWord(uchar *in,uchar *out,
         BYTE LastCod,int fl_all,int HeadLine,SRECT *bnds);
//--SpelNewOneWord--
typedef int (CALLBACK *FUNC_GEN_FULWORD)(ID_SYM id,ZN *z,int MaxZn);
//int  PASC InitOneSpell(FUNC_GEN_FULWORD AddrFuncGenFulWord);
//void PASC CloseOneSpell(void);
int PASC FindOneWord(ID_SYM id,int Reg,char *OutWord);
int PASC FindOneWord1(ZN *zBuf,int NumZnWord, int Reg,char *OutWord);
//--Query FullFile--
int InsertWordFirstAlt(int nc,int ns,int nw,char *s,SRECT *frm);
//int DelWordFul(int nc,int ns,int nw);
//--util--
void bounds_rect(int ii,SRECT *bnd,int nx);
void image_r(SRECT *b);
//---��������� �� ������������
//��������� ����� �����
#define RUS 0
#define LAT 1
//�������� �/�
#define CHIF 0
#define LET 1
//�������/������ ��������
#define UPP 0
#define LOW 1
//��������� ������� ��������� � �����
#define FIRST 0
#define FIRST_NEXT 1
#define MIDDLE 2
#define DELIMITER 3
#define NO_LET 4
//�������� ��������� Action
#define TEST 0
#define CORR 1

#define PRECIS 2

#define MAX_CLUST 64
#define MAX_CLUST_COM 9
#define MAX_CLUST_ONE 9
#define MAX_ELEM_CLUST 19

typedef int (*REGLE)(BYTE kod);

#define FEAT_LET struct h_feat_let
FEAT_LET
{ uint Lang:1; // 0 - R, 1 - E
  uint Let:1;  // 1 - Letter, 0 - ���
  uint Chif:1; // 1 - ����� (0...9 * + - =)
  uint Shift:1;// 0 - Upper Case, 1 - Lower Case
  uint PosUpLine:1;//������� �� ����.�������:=UPP,���� ��������� � LOW,���� �� �������
  uint PosDownLine:1;//������� �� ���.�������:=UPP,���� ��������� � LOW,���� �� �������
  uint ImUppLow:1;//�������� ���������� ������� � ����� ����
  uint DelimSubWord:1;// - ( ) [ ] { } " '
  uint IndCl:6;//������ �������� �������������,���� > 0
  uint Reserv:2;
  //BYTE RusLat;//���� ������� �� ��������� ���� ��.��. ��� 0
};

//int Init_FeatLet(void);
uchar Get1Alt(ZN *z,int na);
int GetFeatLet(ZN *z,int fl_all,int fl_Ext,REGLE Regle);
int R_Chif(BYTE cod);
int R_Let (BYTE cod);
int R_Rus (BYTE cod);
int R_Lat (BYTE cod);
int R_Upp (BYTE cod);
int R_Low (BYTE cod);
int TestWordFeat(ZN *z,int kz, int fl_all,REGLE Regle1, REGLE Regle2);
int TestWordChifLet(ZN *z,int kz,int fl_all);
int TestWordLang(ZN *z,int kz,int fl_all);
int CorrFeat(ZN *z,int kz,REGLE Regle);
int CorrWordChifLet(ZN *z,int kz,int fl_all);
int CorrWordLang(ZN *z,int kz,int fl_all);
int CorrWord(ZN *z,int kz,BYTE LastCod,int fl_all,int HeadLine,SRECT *bnds);
int TestStickUpp(ZN *z,SRECT *bnds);
int GetFeatShift(ZN *z,int fl_all,SRECT *bnds);
int TestWordShift(ZN *z,int kz,int fl_all,SRECT *bnds);
int CorrWordShift(ZN *z,int kz,BYTE LastCod,int fl_all,int fl_bnd,int fl_ExtAlt,
    int Lang,int HeadLine,SRECT *bnds);
int CorrShift(ZN *z,int flUppLow,int fl_bnd,int fl_ExtAlt,int fl_all,int Lang,
    int PosWord,SRECT *bnds);
int CorrWordFnt(ZN *z,int kz,int fl_all);
int TestWordFnt(ZN *z,int kz,int fl_all);
int GetAllowExtAlt(int Action,REGLE Regle);
int CorrFul(void);
int EstRectWordFul(int nc,int ns);
int EstRectWord(SRECT *frm,int ksym,SRECT *bnd);
int AllowRelDistAltOCR(ZN *z,int alt1,int alt2);
//---------RTF-converter----------
//int FullRtf(FILE *fpFileNameFul,char *FileNameOut);
void MyUnionRect(SRECT *s1,SRECT *s2,SRECT *u);

#pragma pack(1)
//#ifdef FUL

#ifndef V_LOCK
  //#define PASCAL
  //#define FAR
  //#include "vocab.h"
#endif

//#endif
#pragma pack(2)
#ifdef MAIN2
/* Moved all these to util_spl.cpp.
  //---�� ������ �������---
  int NumCol,SizeSectionCol;
  int *NumStr;//[nc]
  Word32 *UserNumber;//[nc]
  Word32 *FragFlag;//[nc]
  SRECT *BndCol;//[nc]
  STAT_COL *StatCol;//[nc]
  int len_col,k_frm;
  //int k_colt,slope,space;
  char name[80],name1[LEN_PATH];
  int k_col[MAX_LEV+1],k_lev; LEV **knot; //������
  int fl_EditStruct; //������� �������������� ������ ��������. ��������
  //---�� ������ ���� ���---
  int SizeSectionFntName; char *FntName;
  //---�� ������ ���������---
  ZN ****Zn;//[nc][ns][nw][nz]
  TITLE_STR **TitleStr;//[nc][ns]
  TITLE_WORD ***TitleWord;//[nc][ns][nw]
  TITLE_FUL TitleFul;
  COOR_COMP *CoorComp; long NumComp;
  //--FullSpellFull
  uchar word[MAX_SYM_WORD+1],word1[MAX_SYM_WORD+1];
  float dist[MAX_SYM_WORD+1],dist1[MAX_SYM_WORD+1];
  PAR_FUL par_ful;
  char PunctuationMarks[256];
  int ExtSettings;
  //char AlphabetAllow[256];
  char WordOne[256];
  uchar Upper[256],Lower[256],UppLow[256],UppLowOther[256],EndSentence[256];
  BYTE KoderRus[256],KoderLat[256];
  int FlCarryStr;
  #ifdef FUL
   // svocabg far *ParSpell;
  #endif
  char FileParSpel[MAXFILENAME],FileParSpelOut[MAXFILENAME];
  int *Pen;
  int MaxValue;
  //--��������� �� ������������
  FEAT_LET FeatLet[256];
  BYTE **ClustOCR;int K_Clust;//�������� �������������
  //---WORK---
  BYTE buf[1024],buf1[1024];
*/
#else
  //---�� ������ �������---
  extern Int16 NumCol;
  extern int SizeSectionCol;
  extern Int16 *NumStr;//[nc]
  extern Word32 *UserNumber;//[nc]
  extern Word32 *FragFlag;//[nc]
  extern SRECT *BndCol;//[nc]
  extern STAT_COL *StatCol;//[nc]
  extern int len_col,k_colt,slope,k_frm,space;
  extern char name[80],name1[LEN_PATH];
  extern int k_col[MAX_LEV+1],k_lev; extern LEV **knot; //������
  extern int fl_EditStruct;
  //---�� ������ ���� ���---
  extern int SizeSectionFntName; extern char *FntName;
  //---�� ������ ���������---
  extern ZN ****Zn;//[nc][ns][nw][nz]
  extern TITLE_STR **TitleStr;//[nc][ns]
  extern TITLE_WORD ***TitleWord;//[nc][ns][nw]
  extern TITLE_FUL TitleFul;
  extern COOR_COMP *CoorComp; extern long NumComp;
  //--FullSpellFull
  extern uchar word[MAX_SYM_WORD+1],word1[MAX_SYM_WORD+1];
  extern float dist[MAX_SYM_WORD+1],dist1[MAX_SYM_WORD+1];
  extern PAR_FUL par_ful;
  extern char PunctuationMarks[256];
  extern int ExtSettings;
  //extern char AlphabetAllow[256];
  extern char WordOne[256];
  extern uchar Upper[256],Lower[256],UppLow[256],UppLowOther[256],EndSentence[256];
  extern BYTE KoderRus[256],KoderLat[256];
  extern int FlCarryStr;
  #ifdef FUL
 // extern svocabg far *ParSpell;
  #endif
  extern char FileParSpel[MAXFILENAME],FileParSpelOut[MAXFILENAME];
  extern int *Pen;
  extern int MaxValue;
  //--��������� �� ������������
  extern FEAT_LET FeatLet[256];
  extern BYTE **ClustOCR;extern int K_Clust;
  //---WORK---
  extern BYTE buf[1024],buf1[1024];
#endif
void FreeClustOCR(void);
//#ifdef BLANK
//void ExtrKnot(KNOTT *Curr,KNOTT **back,KNOTT **next);
//void ReverseInsertKnot(KNOTT *Curr,KNOTT *back,KNOTT *next);
//int PASC GenFullTxtfromTree(char *FileNameFul,char *FileNameOut,INF_TREE *Inf);
void PASC OnBreakFrm(int ExtBreak_on);
//int FormatKnot(KNOTT *Knot,SRECT *Bnd,char ***Str,int **Ksym);
//int compKNOTT_RectBottom(KNOTT **a,KNOTT **b);
//int compKNOTT_RectRight(KNOTT **a,KNOTT **b);
int CorrFulCell(KNOTT **AllT,int nT,INF_TREE *Inf);
int CorrCol5_7(int nc);
//#endif
//��������� Ful-�������
// 1.� ��������� ����� ����� ���� ZOffset (�������� ������ ���������)
// ���������� ���� BYTE ProcSpell;//������� ��������� Spell
// 2. ����� ������ OpenFul ������ ���� W_Rect (8 ����) � TitleWord - ������ ����
//TITLE_WORD { BYTE Z_Code; //��������������� ��� ��������� (=1)
//             W_GEN W_Gen; //����������� �����
//             //SRECT W_Rect;//�����
//             int NumAltSpell; //����� ���������
//             int PosFirstCorr;//������� ������ ������ ���� Ins,Del
//             ALT_SPELL *AltSpell;//���������
//             WORD W_Attr; //�������� �����
//           };
//ALT_SPELL { BYTE Len; //����� ��������� � ����������� \0 (���� - � 1)
//            BYTE Reserv;
//            int  Penalty;//����� � �������� SPELL �� ������ ������������
//            char *Alt; //���� ������������ - ������ � ������� �
//          };

#include "undef32.h"

#ifdef __cplusplus
}
#endif

