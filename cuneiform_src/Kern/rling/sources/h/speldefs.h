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


// 08-13-93 08:41pm, Mike
//
// Updating:  Command < #include "speltab.h" > has been added at the
//            bottom of the file.
//
// Additional updating: see 08-13-93
#ifndef __SPEL__DEFS__
#define __SPEL__DEFS__

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include "ligas.h"

// AK removing this stupid macros!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define Q SPQ
//////////////////////////////////////////////////////////////////////////////
// ED tegs
#define SS_BITMAP_REF          0x00
#define SS_TEXT_REF            0x01
#define SS_FONT_KEGL           0x02
#define SS_KEGL                0x03
#define SS_SHIFT               0x04
#define SS_RETR_LEVEL          0x05
#define SS_UNDERLINE           0x06
#define SS_DENS_PRINT          0x07
#define SS_TABUL               0x08
#define SS_TABL_TABUL          0x09
#define SS_SHEET_DESCR         0x0a
#define SS_FRAGMENT            0x0b
#define SS_STEP_BACK           0x0c
#define SS_LINE_BEG            0x0d
#define SS_POSITION            0x0e
#define SS_LANGUAGE            0x0f
#define SS_TABL_CONFORM_SIZES  0x10
#define SS_GROUP_WORDS         0x11
#define SS_GROUP_SYMBOLS       0x12
#define SS_AKSANT              0x1d
#define SS_NEG_HALF_SPACE      0x1e
#define SS_POS_HALF_SPACE      0x1f
///////////////////////////////////////////////////////////////////////////
#define MAX_SHEET              1
#define MAX_FRAGM              128
#define TOO_MANY_FRAGMS        -2
#define MAX_LINE_LTH           1000
#define ALL                    1
#define SIZE_NEW_SEGM          128
#define EXIST                  1
#define NONEXIST               0
#define RIGHT                  1
#define LEFT                   0
#define FRAGM_EL_TYPE          3
#define NUM_ALLOC              10
#define HOME                   0x47
#define END                    0x4f
#define ESC                    0x011B
#define BS                     0x08
#define TAB                    0x09
#define RETURN                 0x0D
#define DEL                    0x53
#define NEW_LINE               1
#define CONT_LINE              2
#define FRAGM                  0
#define SHEET                  1
#define LINE                   2
#define YES                    1
#define NO                     0
#define MAX_DEFAULT_PROB       240
#define MARK_5                 240
#define MARK_4                 200
#define MARK_3                 150
#define MARK_2                 50
#define TIF_COL_COEFF          8
#define TIF_ROW_COEFF          14
#define LAST_LETTER            -1
///////#define BUFSIZE 8192
#define MAX_STRING_LTH         80
#define TEXT_ACTIVE            1
#define TIF_ACTIVE             0
#define BAD_LEVEL              241
#define BSPACE                 0xE
#define DELTA_ATTR             60
#define ED_READ                0
#define ED_WRITE               1
#define SPACE                  0x20
#define NONSPACE               0
#define ON                     1
#define OFF                    0
#define TINY                   1
#define LARGE                  0
#define UPPER                  0
#define LOWER                  1
#define LEFT_MOVE              1
#define RIGHT_MOVE             2
#define UP_MOVE                4
#define DOWN_MOVE              8
#define OFF                    0
#define ON                     1

#define DELTA                  50
/////////////////////////////////////////////////////////////////////////////
#define MAX_PARTS              16       /* max basic parts of object stored*/
#define MAX_WORDS              32       /* max nmb of words to compare    */
#define MAX_WORD_SIZE          32       /* max size of an inputing word   */
#define OK                     1        /* standart reply of a function   */
#define FAIL                   0        /* standart reply of a function   */
//////////////////////////////////////////////////////////////////////////////
//                    E D C O N V   C O N S T A N T S                      //
//////////////////////////////////////////////////////////////////////////////
#define EDBUFF                 1024      /*  edroot - ed     */
#define RECLEV                 0.6       /* num of BLANK / all length */
#define ATDBL                  3
#define MAXWRDBUFF             38
#define BLANK                  (BYTE)bad_char
#define CR                     (BYTE)0xFF
#define TRADEMARK              0X7E
#define WRDDIV                 (BYTE)'-'
#define EOLIST                 (BYTE)0x01
#define User_dicts             4          /* word has to be checked only according
                                           * user dictionaries. */
#define All_dicts              3          /* word has to be checked according all
                                           * available dicts. */
#define Stop                   2          /* proc reply: stop loop routine     */
#define Ok                     1          /* proc reply: OK (cond is satisfied)*/
#define No                     0          /* proc reply: BAD (not satisfied)   */
#define DIG_DEFAULT_FRC        7
#define MAX_DEFAULT_PROB       240
#define MIN_SURE_PROB          240
#define MARK_5                 240
#define MARK_4                 200
#define MARK_3                 150
#define MARK_2                 50
#define MARK_1                 0
#define VRBD                   (DWORD)0x300
#define BADFORDIN              MARK_4      /* level of "bad character"  */
/**************************************************************************/
/* VOCABULARY: constant definitions.                                      */
/**************************************************************************/
#define VOC_W                  1           /* write in vocabulary                 */
#define VOC_R                  2           /* read from vocabulary (search in voc)*/
#ifdef ENGLISH_L
   #define MAX_VOC_SIZE        0xf000      /*  */
#else
   #define MAX_VOC_SIZE        0x6000      /*  24K */
#endif
#define VOC_MEM_OVERFLOW       -2
#define VOC_WRONG_ACCESS       -3
#define VOC_ACCNT_OVERFLOW     -4
#define VOC_ACCNT_ZERO         -5
#define VOC_TOO_SMALL          -6      /* MAX_VOC_SIZE less then sizeof(vert) */
/**************************************************************************/
typedef BYTE byte;

struct bit_map_ref
{
 byte code;
 byte reserv;
 INT row;
 INT col;
 INT width;
 INT height;
};

struct text_ref                        /*type 0- letter     */
{
 byte code;
 byte type;                            /*     2- word       */
 INT object;                           /*     3-string      */
};

struct font_kegl
{
 byte code;
 byte new_kegl;
 INT  new_font;
};

struct kegl
{
 byte code;
 byte new_kegl;
};

struct shift_struct
{
 byte code;
 byte kegl;
 INT clearance;                         /*value of lift or descent */
};                                     /*  lift positive          */
                       /* descent negative*/
struct retrieve_level
{
 byte code;
 byte kegl;
};

                       /* 0 - thin straight */

                       /* 1- half thick straight */
                       /* 2- thick straight      */
                       /* 3- thin cursive        */
                       /* 4- half thick cursive  */
                       /* 5- thick cursive       */
                       /* 6- beg of underline    */
                       /* 7- end of underline    */

struct EdTagLanguage //for russian-english recognize
 {
 BYTE code; // 0x0F
 BYTE language;
 };

struct underline                       /* 0 - straight font            */
{                                      /* 1 - half fat or fat straight */
 byte code;                            /* 2 - thin cursiv              */
 byte type;                            /* 3 -  thick straight          */
};                                     /* 4- beg of underline          */
                       /* 5- end of underline          */
struct dens_print
{
 byte code;
 byte dens_atr;                         /*attribute of print's
                     density*/
};

struct  tabul
{
 byte code;
 byte numb_in_tab_tabul;               /*number of position in*/
};
                     /* table of tabulation*/
struct tabl_tabul
{
 byte code;
 byte lth;                              /* lth- scale of arow*/
 INT arow_pos[1] ;                      /* tabl_tabul->lth ????????*/
};

struct  step_back
{
 byte code;                                       /* step_back-value of backstep*/
 byte step_back;
};

struct  line_beg
{
 byte code;
 byte store;
 INT displacement;                     /*displacement -displ. for cur.*/
};                                     /* line to upper frame of fragm*/

struct  position
{
 byte code;
 byte store;
 INT pos;                              /* pos-position in line for*/
};                                     /* left frame of  fragm*/

struct  tabl_conform_sizes
{
 byte code;
 byte store;
 CHAR tab_val_A [9*2];                 /*tab_val_A-table of sizes*/
};                                      /*   of letter A for kegles */
                    /*  from 4 to 12 */
struct fragm_disk
{
 byte code;
 byte fragm_numb;
 INT depth;                           /*depth -displ. for cur.       */
};                                /*line to upper frame of fragm*/

struct group_words
{
 byte code;
 byte  gr_wd_type;                     /*type-
                          0-beg of group
                          1-cur. word of group
                          2-end
                          3-partition between groups}*/
};

struct  group_symbols
{
 byte code;
 byte  gr_sb_type;
 };

struct  aksant
{
 byte code1;
 byte code;
};

struct  neg_half_space
{
 byte code;
 byte  authenticity_degree;
};

struct pos_half_space
{
 byte code;
 byte authenticity_degree;
};

struct segm
 {                                     /* dynamic memory segment's header */
  CHAR el_type;                        /* = 4 */
  struct segm  *next_in_fragm;     /* next segment in fragment */
  struct segm  *prev_in_fragm;     /* previous segment in fragment */
  struct segm  *next_in_sheet;
  struct segm  *prev_in_sheet;
  INT busy_lth;                        /* busy length of string */
  INT string_lth;                      /* length of string in segm */
  INT q_vis_symb;                      /* quantity of visibal symbols*/
  INT q_v_s_l;                         /* the same in line*/
  CHAR type;                           /* 1- tail of line in the next segm
                                          2- that's tail of line */
  INT font;                            /* font */
  CHAR kegl;                           /* kegl */
  CHAR font_type;                      /* type of font */
  CHAR density;                        /* printing density */
  struct bit_map_ref  map_ref;          /* reference to bit-map */
  INT line_ref;                  /* reference to the line of schedule */
  CHAR store[3];                       /* expand */
  BYTE string[1];                      /* text itself */
 };

struct sheet_descr
{                                     /* sheet header */
 CHAR el_type;                        /* = 2 */
 INT sheet_numb;                      /* sheet number */
 struct segm  *first_segm;      /* address of first segment of first
                                         line in sheet */
 struct sheet_descr  *prev_sheet; /* address of previous sheet */
 struct sheet_descr  *next_sheet; /* address of next sheet */
 struct sheet_descr  *sheet;      /* address of sheet */
 BYTE quant_fragm;                      /* quant. of fragments in sheet */
 INT lth;                               /*length of descriptor with fragm*/
 struct fragm_descr *fragm[256];  /* addresses of sheet's fragments */
};

struct file_descr
{                                     /* file header */
 CHAR el_type;                        /* = 1 */
 struct sheet_descr * first_sheet;  /* address of first sheet of file */
 INT file_handle;                     /* file handle */
 CHAR file_name[40];                  /* file name */
 CHAR  *pool_start;               /* adrress of pool for page */
 LONG  pool_lth;                 /* Q.w_lenth of pool (in bytes) */
 INT n_free;                   /* number of free segments in pool */
 CHAR  *pool_free;      /* address of first free block in pool */
};

struct sheet_disk_descr
{
 byte code;
 BYTE quant_fragm;
 INT sheet_numb;
 INT descr_lth;
 BYTE byte_flag;
 CHAR tabl[17];
};

#define FD_TYPE_TEXT 0
#define FD_TYPE_PICT 1
#define FD_TYPE_TABLE 2
#define FD_TYPE_EMPTY 3

struct fragm_descr
 {                                     /* fragment header */
  CHAR el_type;                        /* = 3 */
  INT row;                             /* coordinates of left upper */
  INT col;                             /* angle of fragment's frame */
  INT height;                          /* height of fragment */
  INT w_width;                         /* Q.w_width of fragment */
  CHAR type;                           /* fragment's type */
  byte kegl;
  byte font;
  byte font_type;
  struct segm  *first_segm;        /* address of first segm (line) */
  struct segm  *last_segm;         /* address of last segm */
 };

struct fragm_disk_descr
 {
  byte code;
  INT row;                             /* coordinates of left upper */
  INT col;                             /* angle of fragment's frame */
  INT height;                          /* height of fragment */
  INT w_width;                         /* Q.w_width of fragment */
  CHAR type;
  byte kegl;                           /*kegl for following fragm*/
  byte font;                            /*font name number~~~~~~~~*/
  byte language;                        // 03-29-94 02:42pm,Pit
  byte  font_type;                     /*type specifications of font
                     for additional information
                     look at underline */
 };

struct int_row_col
 {
  INT row;
  INT col;
 };

struct state_stack_elem
 {                                     /* element of state_stack */
  CHAR stack_type;                     /* ??????? */
  INT line_row;                        /* ??????? */
  INT position;
  byte font;                            /* font */
  byte language;                        // language for fragment
  CHAR kegl;                           /* kegl */
  CHAR font_type;                      /* font type */
  CHAR density;                        /* printing density */
  CHAR underline;                      /* presence of underline */
  struct bit_map_ref map_ref;          /* address of cur_symb in bit_map */
 BYTE  *ns_ref_ptr;       /*address of current map_ref*/
  INT letter_ref;                      /* ??????? */
  INT halo_ref;                        /* ??????? */
  INT word_ref;                        /*  */
  INT line_ref;                        /* numb of symb incl. all symb */
  struct state_stack_elem  *next;  /*addres of stack next level*/
 };

struct letter
 {
  byte code;
  byte attr;
 };

typedef struct state
 {                        /* description of state characteristics */
  struct segm *cur_segm;          /* address of current segm */
  BYTE  *cur_symb;            /* address of current non control symbol */
  CHAR stack_level;           /* level of state stack */
  CHAR size_tab[18];                   /* values of tabulation */
  INT position;                        /* ?????? */
  INT pos_line;                   /* number of symb including all symb. */
  struct file_descr  *file;        /* ?????? */
  struct sheet_descr  *sheet;      /* ?????? */

 } dst;

typedef struct tab
 {
  struct sheet_descr tab_sheet_descr[MAX_SHEET];
  struct fragm_descr tab_fragm[MAX_SHEET][MAX_FRAGM];
 } dt;

/* 21-02-93 10:55pm, Mike */
/*  I have exclude static dictionary internal structures from this file   */
/*  and have create new header file SPELLDICT.H.                          */

#include "speldict.h"

/***************************************************************************/
/*          data types & definitions for edconv()                          */
/***************************************************************************/

typedef struct letter LT;               /* code & probability of letter     */

#define POS 0

typedef struct stdhlt {
/*    POS stdh;   */                     /*header                          */
    struct letter stdlt[ABCSIZE];         /*standard alphabetic list (const)*/
     } STD;

struct ltimg {
    struct letter  *lt;                    /* ref to curr LT in list              */
    BYTE  *source;     /* reference to header & list          */
    STD std;                        /* ref to standard header & list       */
    INT blank;                      /* =1, if alternatives are not set     */
            /* alt indices in respective pos (obj->pos[i].alt[]): */
    BYTE altn[ABCSIZE];
               /* all pos[] for the wrdimg are in obj->part->pos   */
           /* if blank=1, altn[]={0,1,2,...,ABCSIZE}                   */
      } ;
 typedef struct ltimg LTIMG;
/***************************************************************************/
/*                                                                         */
/***************************************************************************/

/* VSTRUCT.H                                                              */
/**************************************************************************/
/* DYNAMIC VOCABULARY : data structure                                             */
/**************************************************************************/

/***************************************************************************/
/********  Word description in voc structure                      **********/
/***************************************************************************/
typedef struct word_descr_in_voc
     {  WORD                 next;
    INT                   nmb;
    struct segm   *segm;
    CHAR          *symb;
    BYTE                 type;
    BYTE              type_sp;
    BYTE                  voc;
    BYTE               l_cont;
    BYTE               r_cont;
      } WTOKEN;
/*********************      CONTEXT FLAGS      *****************************/

#define  C_UNREC  0x01        /* Left/right word has unrecognized context  */
#define  C_STBEG  0x02        /* Word is the first in statement            */
#define  C_STEND  0x02        /* Word is the last in statement             */
#define  C_FIRST  0x04        /* Left word is the first in statement       */
#define  C_LOW    0x08        /* Left/right word consists of low letters   */
#define  C_CAP    0x10        /* Left/right word consists of Cap letters   */
#define  C_NAME   0x20        /* Left/right word is name                   */
#define  C_MIXED  0x40        /* Left/right word consists of Cap,low,digits*/
#define  C_DIG    0x80        /* Left/right word is pure digital           */

/***********************      WORD   FLAGS     *****************************/

#define  W_BAD    0x01       /*  Word has unrecognized symbols             */
#define  W_DELIM  0x02       /*  Word has delimeters                       */
#define  W_CHECK  0x04       /*  Means that it is worth to process this wrd*/
                 /*  during the second pass                    */
#define  W_LOW    0x08       /*  Word consists of totally low letters      */
#define  W_CAP    0x10       /*  Word consists of totally capital letters  */
#define  W_NAME   0x20       /*  Word has first Cap & other low letters and*/
                 /*  there is no statm  terminators before it  */
#define  W_MIXED  0x40       /*  Word consists of Cap,low digit and it is  */
                 /*  not name or first word in statement       */
#define  W_DIG    0x80       /*  Word consists of totally digits.          */

/***************************************************************************/
/*           Vocabulary vertex structure ( 8 bytes , near pointers )       */
/***************************************************************************/

struct v {
  BYTE key;                  /*  key of vertex                    */
  BYTE wrdterm :1;            /*  flag of word termination         */
  BYTE accnt :7;              /*  statistics account               */
  WORD left;                  /*  pointer to left brother          */
  WORD right;                 /*  pointer to right brother         */
  WORD down;                  /*  pointer to down brother ( son )  */
  WORD wtoken;
    };

typedef struct v vert;                /* type of vocabulary vertexes       */

/***************************************************************************/
/* Vocabulary gobal var's structure ( MAX_WORD_SIZE*2+6 bytes )            */
/***************************************************************************/

#define DYN_DICT_HEADER "CTC USD"

struct vstate
{
	INT lev;                            /* current level in voc. 0 : M_W_S-1 */
	WORD path [MAX_WORD_SIZE];  /* path from root to current vertex  */
	/*  WORD vocseg;    */        /* base register of vocabulary       */

	BYTE * vocseg;
	WORD vocfree;               /* size of unused space in voc.      */
	BYTE state;
};
                  /*           Voc states definitions */
#define VOC_CHANGED  0x01

typedef struct vstate voc_state;
/**************************************************************************/

#define MAX_VOC_NUMBER 2

typedef struct
{
  CHAR       voc_name[1];
  voc_state  voc;
  INT        type;

} user_voc;


/***************************************************************************/
/*                                                                         */
/***************************************************************************/
/* CONDDEF>H */   /* Constants for dictionary sgueeze */
/*
#define  MAX_WORD_SIZE 32
*/
#define  VOC_DOWN  1
#define  VOC_RIGHT 2
#define  VOC_UP    3
#define  NO        0
#define  MAX_PAGE  (long)(MAX_VOC_SIZE/3*2)
#define  NPCDEF    60
/*======================================================================*/

 struct super_ed
  {
  INT x;
  INT y;
  INT x_a;
  INT y_a;
  INT miss;
  INT f_e;
  INT flag;
  INT col;
  };

  struct super_next
  {
   WORD rec;
   WORD beg_next;
   WORD last_altern;
   WORD end_altern;
   WORD st;
   };



typedef struct super
 {
  WORD tif_top_y;
  WORD tif_small_top_y;
  struct int_row_col tif_pos;
  WORD active_window;
  WORD beg_sheet;
  WORD beg_next;
  WORD real_num;
  WORD move;
  INT add_row;
  INT add_col;
  INT add_width;
  INT add_height;
  WORD corner;
   WORD end_altern;
  WORD last_altern;
  BYTE text_line[sizeof(struct letter)*MAX_STRING_LTH];  //  2 * 80
  dt  *tab_ptr;
  CHAR * text_buff;
  //////// CHAR text_buff_pool[BUFSIZE];   // 8192
  CHAR ins_status;
  CHAR  *beg_alloc_mem;
  CHAR  *end_alloc_mem;
  CHAR  *free_alloc_mem;
  CHAR  *ptr_tab;
  WORD  cur_fragm;
  WORD qt_fm;
  WORD  cur_sheet;
  WORD  cur_sheet_number;
  WORD  stack_level;
  struct segm  * last_del_line;
  struct segm  *ptr_cur_segm;
  struct segm  *ptr_next_segm;
  BYTE *buff_ptr;
  struct segm  *new_segm;
  CHAR atr;
  CHAR bgc;
  LONG buff_l;
  LONG l;                               /*length of done way in next_symb*/
  WORD new_s;                       /*sign of new segm of curr line*/
  CHAR ed_fname[6];
  INT ed_file;
  LONG file_lth;
  INT start_sheet_number;
  CHAR  *str_ptr;
  WORD ram_change_status;
  WORD end_l;
  WORD end_s;
  WORD curs_x_a;
  WORD curs_y_a;
  WORD curs_x_w;
  WORD curs_y_w;
  WORD w_x;
  WORD w_y;
  WORD w_width;
  WORD w_height;
  WORD tif_x;                      /*coordinates of tif window */
  WORD tif_y;
  WORD tif_width;
  WORD tif_height;
  INT tif_file;
  CHAR tif_file_name[40];
  INT tif_line_lth;
  INT tif_n_lines;
  WORD large_height;
  WORD small_height;
  INT q_pg_right; /* quantity of pages that we have passed moving right */
  WORD sw_symb;/* sw_symb = 1 shows that was recurs. call of next_symb()*/
  WORD esc_n_s;
  CHAR fragm_flag[MAX_FRAGM];
  CHAR sheet_flag[MAX_SHEET];
  struct bit_map_ref *bit_map_ref_ptr;
  struct bit_map_ref  *h_bit_map_ref_ptr;
  struct text_ref *text_ref_ptr;
  struct text_ref  *h_text_ref_ptr;
  struct font_kegl *font_kegl_ptr;
  struct font_kegl  *h_font_kegl_ptr;
  struct kegl *kegl_ptr;
  struct kegl  *h_kegl_ptr;
  struct shift_struct *shift_ptr;
  struct shift_struct  *h_shift_ptr;
  struct underline *underline_ptr;
  struct underline  *h_underline_ptr;
  struct dens_print *dens_print_ptr;
  struct dens_print  *h_dens_print_ptr;
  struct position  *position_ptr;
  struct position  *h_position_ptr;
  struct fragm_disk *fragm_disk_ptr;
  struct fragm_disk  *h_fragm_disk_ptr;
  struct fragm_disk_descr *fragm_disk_descr_ptr;
  struct fragm_disk_descr  *h_fragm_disk_descr_ptr;
  struct sheet_disk_descr *sheet_disk_descr_ptr;
  struct sheet_disk_descr  *h_sheet_disk_descr_ptr;
  struct state_stack_elem  *stack;
  LONG alloc_size;
  INT shift_status;
  INT key;
  struct segm  *ns_segm;        /* segm_ptr corresponding to next_symb() */
  BYTE  *ns_symb;      /* symbol corresponding to next_symb() */
  INT skip_line_x;
  INT flag_out;
  CHAR edit_file_name[40];
  INT end_page_numb;
  CHAR stack_change_flag;
  INT word_attr;
  INT tiny_small_height;
  INT tiny_w_x;
  INT tiny_w_y;
  INT tiny_w_width;
  INT tiny_w_height;
  INT tiny_large_height;
  INT tiny_tif_x;
  INT tiny_tif_small_top_y;
  INT tiny_tif_top_y;
  INT tiny_tif_width;
  INT tiny_tif_height;
  INT tiny_top_y;
  INT w_status;
  INT big_small_height;
  INT big_w_x;
  INT big_w_y;
  INT big_w_width;
  INT big_w_height;
  INT big_large_height;
  INT big_tif_x;
  INT big_tif_small_top_y;
  INT big_tif_top_y;
  INT big_tif_width;
  INT big_tif_height;
  INT save_flag;
  struct dict_state d_state;
 /* voc_state v_state;      */
 /* CHAR vocfile [40];      */
 /* CHAR temp_voc_file[40]; */
  INT conv_call;
  CHAR  *w_c;
  CHAR check_word[40];
  CHAR right_part[20];
  CHAR* r_p;
  CHAR *buff_out;
  INT ed_out;
  ////// CHAR text_buff_out[BUFSIZE];  // 8192
  CHAR word_out[6][20];
  CHAR* w_out;
  dst st;
  dt   t;
  struct super_ed E;
  struct super_next N;
 } dQ;
 //AK
 //#define st Q.st
 //#define E Q.E
 //#define t Q.t
 //#define N Q.N



 /**************************************************************************/
 /*****************  defines & structures for spelling itself (tg_spell) ***/
 /**************************************************************************/
 struct tre_state
        {
     WORD             font;
     WORD             kegl;
     WORD        font_type;
     CHAR      file_name[40];
     BYTE *   buf;
     WORD         buf_size;
     WORD              lth;
        };
#define TRE_BUF_SIZE 0xA000
  struct tif_state
          {
       WORD          shift;    /*  shift (in tif lines ) */
       BYTE  *buf;
       WORD       buf_size;
       WORD            lth;
     } ;
#define TIF_BUF_SIZE 0x7000
#ifdef OVERVOC
 #define CHECKVOC  check_voc()
 #define CHECKTRE  check_tre()
 #define CHECKTIF  check_tif()
#else
 #define CHECKVOC  0
 #define CHECKTRE  0
 #define CHECKTIF  0
#endif

#define TEMPVOCFILE "t:\\tmp.voc"
 /*******************************************************************/

       /*  Constants for fill_raster()         */

#define CONV_MAP   1           /*      invert   source raster          */
#define NOCONV_MAP 0           /*  not invert    ---""---              */


#define FIR_CHECKED 1               /*             spelling is done    */

CHAR  *find_byte_flag(void);

/* ======================== */
  /* max size of an object view field (max nmb of positions) : */

#define MAX_VIEW_SIZE 28
/*
#define MAX_VIEW_SIZE 16
*/
  /* special position number used for to store artificialy constructed alts:*/
#define SPEC_POS MAX_VIEW_SIZE
  /* min difference in probabilities to consider alts to be similar: */
#define MIN_DIFF_PROB 8
  /* min prob to consider alt, pos,...  */
#define PROB_MIN 100
  /* prob allowed to use the estimation of alt, pos,...  */
#define PROB_ALLOWED 199
  /* min prob to be sure in the back_recog result (BOX) */
#define PROB_SURE 200
  /* ...... ??       */
#define PROB_BLUE 150
//////////////////////////////////////////////////////////////////////////////
// Diferences for correct words
// Allex 26.04.99             rise MD_for_not_name
#define PROB_TO_BRK           164   /* 152 */
#define MD_for_name           50   /*  40 */
#define MD_for_not_name       50   /*50 Lepik, 27 dec 93, 110  */
#define MD_for_one_char       25
#define MD_for_first_in_name  60
#define MD_for_first          50
//////////////////////////////////////////////////////////////////////////////
#define DIF_for_stat_repl     2 /* if difference between attributes of     */
                                /* alternatives is less than this one,     */
                                /* alternatives may be subsituted one by another*/
/* ======================== */

struct tifref
 {
  WORD row;                   /* envelope specification  */
  WORD col;                   /*  ...                    */
  WORD width;                 /*  ...                    */
  WORD height;                /*  ...                    */
  BYTE *env_ref;    /* ref to envelope specification in tif */
  struct segm * segm;        /*        Q.ns.segm        */
  CHAR *symb;               /*        Q.ns.symb        */
 };

/* ===================================================================== */
/*              defines for spelling checker's FLAGS :                   */
/* ===================================================================== */
  /* ASCII-codes' identification (see all_lt in spellfun.c) : */
#define _BLK 2
#define _SPC 3
#define _WDV 4
#define _TRM 5
#define E_CP 6
#define E_OD 7
#define R_CP 8
#define R_OD 9
#define _APF 10
#define _DIG 11
#define _SP1 12
#define _SP2 13
#define E_DL 14

  /* context flags (type): */
#define T_CHEESE   0x01   /* this means part might  be a quantity */
#define T_ALPHA    0x02   /* all alphabetic              */
#define T_DIG      0x04   /* all digital                 */
#define T_DELIM    0x08   /* an alt-delimiter            */
#define T_CAP      0x10   /* all capitals must be or are */
#define T_LOW      0x20   /* all low case must be or are */
#define T_STMT     0x40   /* beg of sentence             */
#define T_NAME     0x80   /* name                        */

  /* newline, blank, alts, space, hyphen flags (type_sp): */
#define T_LINE     0x01   /* new line begins here                      */
#define T_BLANK    0x02   /* blank (originally)                        */
#define T_MYBLANK  0x04   /* there is a blank (T_BLANK) assigned by me */
#define T_SP1      0x08   /* suspected space&it's available            */
#define T_SP2      0x10   /* suspected space&it's not avail            */
#define T_HYPHEN   0x20   /* hyphen                                    */
#define T_MIXED    0x40   /* mixed (alphabetic+digital)  */

// 08-13-93 09:11pm, Mike
// Added after Lepik.
#define T_APF      0x80   /* apostrof ' occured                        */


  /* change, spec repl, glue-cut, alts  - flags (type_art): */
#define T_CHANGE   0x01   /* word change (only within dict intewrface) */
#define T_REPL     0x02   /* special replacement                       */
#define T_GC       0x04   /* glue-cut                                  */
#define T_ALTS     0x08   /* alts available or not 1-st alts are considered*/
#define T_BRK      0x10   /* fall a badly recognized symbol into  blank */
/* ===================================================================== */
/*               Spelling Checker Data Structure Chart                   */
/* ===================================================================== */
/*
    ROOTS:      obj (objstr)             part (partstr)     word (wrdstr)
    ------      ---                       [MAX_PARTS]       (dynamic list,
                     --------------       see below)
        obj info (blanks,...)    [1] types info      -------------
                         beg, end pos
        pos_part [ 0,x,x,N ]         pos[]: pos-list
                 |           art[]: art-id-list
        pos_part_nmb--->-             wrd (wordstr *) ->---                    |
        part (current, ref)->--  [2]  .....                |
        part_nmb (curr,nmb)->--|   ........                |
        part_max (max nmb, M)  |   ........                |
                   |    -->........                |
        pos (posstr)       |                               |
      [MAX_VIEW_SIZE+MAX_ART_SIZE]  -->  [M]  .....                |
    [1]   pos info :                                           |
         type                                              |
         orig_lt                                           |
         cur_alt                                           |
         tif_ref                                           |
         repl & glue-cut info                                     |
         .......                                           |
         alt_nmb [K]                                       |
         alt (altstr)                                      |
          [ABCSIZE]                                        |
          [1] alt info :                                       |
        * lt ------------> { ltr, prob }                   |
          orig sequential nmb in alt-list                  |
          orig prob                                        |
          type                                             |
          [2] ...........                                      |
          ...............                                      |
          [K] ...........                                      |
                                   |
    [2]   pos info                                             |
        ......                                             |
          [1] alt info                                         |
          ............                                         |
          [K] ........                                         |
                                   |
    .....................................                      |
   -->  .....................................                      |
  |     .....................................                      |
  |                                                                |
  |     [N]   pos info                                             |
  |           ......                                               |
  |           [1] alt info                                         |
  |           ............                                         |
  |           [k] ........                                         |
  |                                                                |
  |    *********************************************************   |
  |                                                                |
  |               word (wordstr)                                   |
  |       (dynamic list, pointers to imets are:  part[i]->wrd)     |
  |              ---------------                                   |
  |          <1>  next (wordstr *,    --->--------------           |
  |                     NULL for the last item)         |          |
  |               word info:                            |          |
  |                 type                                |          |
  |                 lth                                 |          |
  |                 voc prob                            |          |
  |                 estimations                         |          |
  |                 altn[]: alt-num-list                |          |
  |                 alt prob (descending order)         |          |
  |                                                     |          |
  |               pos (posstr *)                        |          |
  |              [MAX_WORD_SIZE]                        |          |
  |                [1] ..........                       |          |
   -----------<---     ..........                       |          |
           [P] ..........                       |          |
                            |          |
         <2>  ...............  <--------------------           |
         ....................                                  |
         <X>  .............  <---------------------------------
         ....................

         <Z>  ...............


  Note: P = part.end-part.end+1;

*/
/* ===================================================================== */
/*   object (set of positions) & values (alternatives) management info   */
/* ===================================================================== */


/*  for each alternative:                              */
/* --------------------------------------------------- */

 typedef struct altstr {

    struct letter orig;    /* original code & probability              */
    struct letter  * lt; /* ref to alt & prob                      */
    BYTE seqnum;  /* sequential nmb in original alt list      */
                   /* 0, if was not available (then generated) */
    INT dif_wt;            /* weight of discrepancy with orig alts     */
/* ???
    WORD prob_BOX;
   ??? */
    BYTE type;
                   /* T_ALPHA - alphabetic   */
                   /* T_DIG   - digital      */
                   /* T_CAP   - capital      */
                   /* T_LOW   - low case     */
/*    BYTE type_sp;*/              /* T_DELIM  */
        } SALT;


/* ================================================= */
/* for each position                                 */
/* ------------------------------------------------- */

   typedef struct posstr {

    struct letter orig;         /* original ltr & prob                  */
    struct letter  * lt;    /* ref to current ltr & prob            */
    INT dif_wt;                 /* weight of discrepancy with orig alts */
/* ???
    struct matrBOX e_BOX;
     ??? */
                    /*   context info:   */
    BYTE type;
                    /* T_ALPHA - all alts alphabetic   */
                    /* T_DIG   - all alts digital      */
                    /* T_MIXED - mixed (alphabet+digit)*/
                    /* T_CAP   - all capitals          */
                    /* T_LOW   - all low case          */
                    /* T_STMT  - beg of sentence       */
                    /* T_NAME  - name                  */

                    /*   blank, alts, space, hyphen info: */
    BYTE type_sp;      /* T_LINE - new line begins here         */
                    /* T_BLANK   - blank (originally)        */
                    /* T_MYBLANK - T_BLANK is assigned by me */
                    /* T_ALTS  - alternatives available      */
                    /* T_SP1 - suspected space&it's available*/
                    /* T_SP2 - suspected space&it's not avail*/
                    /* T_HYPHEN - hyphen                     */
                    /* T_DELIM - an alt-delimiter takes place*/

    BYTE type_art; /* artificial change (repl & glue-cut) flags:*/
                    /* T_REPL - special replacement          */
                    /* T_GC   - glue-cut                     */
    BYTE art;          /* artificial change id                  */

    float prop_ind;             /*  proportionality index          */
    struct tifref tif_ref;      /* tiff specifications             */
    INT alt_nmb;                /* nmb of alternatives             */
    struct altstr alt[ABCSIZE]; /* list of alternatives' info      */

            } SPOS;

/* ================================================ */

/* for a set of positions (basic object):           */
/* ---------------------------------------          */

typedef struct objstr {
                   /* object's context specification:    */
    BYTE type;
                  /* T_ALPHA -   pure alphabetic          */
                  /* T_DIG   - pure digital               */
                  /* T_MIXED - mixed (alphabetic+digital) */
                  /* T_CAP   - all capitals               */
                  /* T_LOW   - all low case               */
                  /* T_STMT - beg of sentence             */
                  /* T_NAME - name                        */

                  /*   blank, alts, space, hyphen info:  */
    BYTE type_sp;    /* T_LINE - new line begins here            */
                  /* T_BLANK   - blanks (originally) exist    */
                  /* T_MYBLANK - blank assigned by me exist   */
                  /* T_ALTS - alternatives available          */
                  /* T_SP1 - suspected space & it's available */
                  /* T_SP2 - suspected space & it's not avail */
                  /* T_HYPHEN - hyphen exists                 */

    INT nmb;                  /* obj number in the input flow (read)      */
    struct tifref tif_ref;    /* tiff specification                       */
    INT pos_line;             /* new line position (if type_sp|=T_LINE)   */
    INT pos_hyphen;           /* hyphen position                          */
    INT blank_nmb;            /* nmb of blanks                            */
    INT pos_myblank;          /* position of blank generated by me, if any*/
    INT dif_wt;               /* weight of discrepancy with original alts */
    INT alt_nmb;              /* nmb of alternatives                      */

    struct posstr pos [MAX_VIEW_SIZE+1];/* positions: the last one is SPEC*/

    INT pos_part [MAX_WORD_SIZE]; /* list of object partitioning positions:
                   0,N       - if no partitioning,
                   0,x,x,x,N - if partitioning available
                   (because of hyphens & suspected spaces) */
    INT pos_part_nmb;         /* nmb of elements in the list above         */


   /* current state of the object: */
   /* ---------------------------- */

    struct partstr * part; /* ref to current item in part[]                 */
    INT part_nmb;      /* current item number in part[]                     */
    INT part_max;      /* max part number in part-array                     */
    INT part_beg;      /* curr part beg pos                                 */
    INT part_end;      /* curr part end pos                                 */
    INT part_begi;     /* beg index in obj->pos_part[] */
    INT part_endi;     /* end index in obj->pos_part[] */
    INT opt_part [MAX_WORD_SIZE]; /* list of best obj partitioning positions:
                   it is the copy of pos_part[], but
                   0,..x,0,x,0,x,..N - partitioning, so that
                    0 => no end & beg of parts,
                    x => end & beg of best parts chosen     */
    INT opt_mark;      /* mark of the best part chain, specified by opt_part*/
    INT opt_pen;       /* penalty of the best part chain                    */
    INT opt_entire;    /* nonzero, if the opt part-chain is entire          */
    INT voc_kind;      /* kind of voc where the word found:                 */
               /* 2 - dict (static)                                 */
               /* 1 - voc  (dynamic)                                */
    INT lthok;         /* lth of curr word piece found in dict/voc          */
    INT allowedlth;

    BYTE type_art;  /* artificial change (repl & glue-cut) kind:   */
               /* T_REPL - special replacement                      */
               /* T_GC   - glue-cut                                 */
    INT art [MAX_VIEW_SIZE]; /* artificial change identifiers               */
    INT artn;          /* curr index in art[artn]                           */
    INT min_art;       /* curr min art id (left  bound of artbase)          */
    INT max_art;       /* curr max art id (right bound of artbase)          */
    INT art_minpos;    /* min pos of obj->pos[] curr used for part-changes  */

    BYTE type_orig; /* original type (context) of curr part        */
    struct wordstr * word; /* ref to current word under treatment           */
    BYTE wordchar [MAX_WORD_SIZE]; /* word as a char-chain         */
    INT nmb_wrdfound;  /* a nmb of words found for a current part           */

          } SOBJ;

 /*                 Reading object status                                  */
 #define S_NEWOBJ 0
 #define S_OLDOBJ 1

/* ============================================================ */
/*  words to choose the best:                                   */
/*    (word - a certain set of positions' representatives)      */
/* ============================================================ */

/*  for a word:                                    */
/* ----------------------------------------------- */

 typedef struct wordstr {

      struct wordstr * next;           /* ref to next word in part-word-list */
      struct posstr * pos[MAX_WORD_SIZE]; /* refs to obj pos-s               */
      BYTE altn [MAX_WORD_SIZE]; /* alt numbs for correspondent poss*/
      BYTE alt_doubt [MAX_WORD_SIZE]; /* alt doubt values, received */
                    /* by back_recog (for corresp pos-s  */
      BYTE type;
                  /* T_ALPHA - all alts alphabetic       */
                  /* T_DIG   - all alts digital          */
                  /* T_CAP   - all capitals (in fact)    */
                  /* T_LOW   - all low case (in fact)    */
      BYTE type_sp;
                 /* T_BLANK   - blank (originally)        */
                 /* T_DELIM - an alt-delimiter takes place*/
      INT lth;                /* word length                         */
      INT voc_prob;           /* voc (static or dynamic) probability */
      INT voc_kind;           /* kind of voc where the word found:   */
                  /* 3 - pure digital word (consid.to be in voc) */
                  /* 2 - dict (static)                   */
                  /* 1 - voc  (dynamic)                  */
      BYTE type_art; /* artificial change (repl & glue-cut) kind:*/
                  /* T_REPL - special replacement take place  */
                  /* T_GC   - glue-cut take place             */
      BYTE art [MAX_WORD_SIZE]; /* artificial change identifiers */

      INT dif_wt;             /* weight of discrepancy with original alts */
      INT low_alt_nmb;        /* nmb of alts with prob < PROB_ALLOWED */
      INT bad_alt_nmb;        /* nmb of alts with prob < PROB_MIN     */
/*??? INT gen_alt_nmb;*/      /* nmb of alts generated from blanks    */
      INT mark;               /* generalized word estimation          */
      INT mark_doubt;         /* estimation of word's uncertainty :   */
                  /* is set by post-recog when the word   */
                  /* isn't the best by mark               */
         } SWORD;

/* ============================================================ */
/*  best words within object parts:                             */
/*  (partitioning may be caused by hyphens & suspected spaces)  */
/* ============================================================ */

typedef struct partstr
 {
  INT beg;                        /* part id: beg position    */
  INT end;                        /* part id: end position    */
  INT begi;                       /* beg index in obj->pos_part[] */
  INT endi;                       /* end index in obj->pos_part[] */
  BYTE posn[MAX_WORD_SIZE]; /* part positions - list */
  INT lth;                        /* nmb of elements in the list above    */
                  /* part context specification:          */
  BYTE type;             /* T_CHEESE   - quantity ( set of digits suspected.)*/
                  /* T_ALPHA -   pure alphabetic          */
                  /* T_DIG   - pure digital               */
                  /* T_MIXED - mixed (alphabetic+digital) */
                  /* T_CAP   - all capitals               */
                  /* T_LOW   - all low case               */
                  /* T_STMT - beg of sentence             */
                  /* T_NAME - name                        */

                  /*   blank, alts, space, hyphen info:  */
                  /* only blanks are valuable for a part */
  BYTE type_sp;      /* T_LINE - new line begins here            */
                  /* T_BLANK   - blanks (originally) exist    */
                  /* T_MYBLANK - blank assigned by me exist   */
                  /* T_ALTS - alternatives available          */
                  /* T_SP1 - suspected space & it's available */
                  /* T_SP2 - suspected space & it's not avail */
                  /* T_HYPHEN - hyphen exists                 */

  INT blank_nmb;              /* nmb of blanks                            */
  INT dif_wt;                 /* weight of discrepancy with original alts */
  INT alt_nmb;                /* nmb of alternatives                      */

  BYTE type_art;     /* artificial change (repl & glue-cut) kind:*/
                  /* T_REPL - special replacement take place  */
                  /* T_GC   - glue-cut take place             */
  BYTE art [MAX_WORD_SIZE]; /* artificial change identifiers     */

  struct wordstr * word;      /* ref to curr word: last is 1-st in list   */
 }  SPART;

/* ============================================================ */
/*  struct of special replacements & glue-cut definition:       */
/* ============================================================ */
// 08-13-93 08:50pm, Mike
//
//typedef struct artstr
// {
//  BYTE sr1;       /* code1 of what replace */
//  BYTE sr2;       /* code2 of what replace */
//  BYTE obj1;      /* code1 by what replace */
//  BYTE obj2;      /* code2 by what replace */
//  BYTE objt1;     /* type of obj1          */
//  BYTE objt2;     /* type of obj2          */
//  BYTE objts1;    /* type_sp of obj1       */
//  BYTE objts2;    /* type_sp of obj2       */
//  BYTE cond_sr1;  /* cond1 for sr1         */
//  BYTE cond_sr2;  /* cond2 for sr2         */
//  BYTE cond_sr12; /* cond12 for sr1&sr2    */
//  BYTE cond_obj12x;/* cond12 for obj1&obj2 to check before */
//  BYTE cond_obj12r;/* cond12 for obj1&obj2 to check after  */
// } ARTS;

#define MAX_ARTS 41

/* ====================================================================== */
/* Multipliers for estimation of object, word, positions, letters         */
/* They are used both for object & words rejection and making best choice */
/* ====================================================================== */
/*                      Object rejection multipliers:                     */
/* ====================================================================== */
#define RMobj_no        67
#define RMobj_max_blank 3
#define RMobj_min_lth   2

#define MINlth_for_space 3      /* Min lth of part to convert appropriate   */
                /* Susp space into space if another bounding*/
                /* part was not found.                     */


#define RMobj_blank_pen 100
#define RMobj_spsp_pen  40
/* ====================================================================== */
/*                      Part rejection multipliers:                       */
/* ====================================================================== */
#define RMpart_no   49
#define RMpart_max_blank  RMobj_max_blank
#define RMpart_minlth_low RMobj_min_lth

#define RMpart_minlth_cap RMobj_min_lth+1
#define RMpart_minlth_nam RMobj_min_lth+1
/*
#define RMpart_minlth_cap RMobj_min_lth
#define RMpart_minlth_nam RMobj_min_lth
*/
#define RMpart_blank_pen 80

#define RMpart_artno  45
#define RMpart_art_max_blank 2
#define RMpart_art_minlth_low RMobj_min_lth+1
#define RMpart_art_minlth_cap RMobj_min_lth+2
#define RMpart_art_minlth_nam RMobj_min_lth+2
/* ====================================================================== */
/*                      Word rejection multipliers:                       */
/* ====================================================================== */
#define RMwrd_stop 261
#define RMwrd_no   241
#define RMwrd_minlth_low RMobj_min_lth

#define RMwrd_minlth_cap RMobj_min_lth+1
#define RMwrd_minlth_nam RMobj_min_lth+1
 /*
#define RMwrd_minlth_cap RMobj_min_lth
#define RMwrd_minlth_nam RMobj_min_lth
  */
#define RMwrd_bad_alt_nmb 80
#define RMwrd_low_alt_nmb  40
#define RMwrd_dif_alt_wt   20
#define RMwrd_repl_alt     10  /* negative: special replacement done      */
#define RMwrd_gc_alt       15  /* negative: glue-cut            done      */
/* ====================================================================== */
/*                      Object evaluation multipliers:                    */
/* ====================================================================== */
#define VMobj_blank      50    /* negative: blanks exist */
#define VMobj_spsp       50    /* negative               */
#define VMobj_hyphen     50    /* negative               */
#define VMobj_gc         30    /* negative               */
#define VMobj_alt_nmb     1    /* negative: total nmb    */
/* ====================================================================== */
/*                      Word evaluation multipliers:                      */
/* ====================================================================== */
#define VMwrd_stop 61       /* negative */
#define VMwrd_no   35       /* negative */
#define VMwrd_min  15       /* positive - negative (min to accept)     */
#define VMwrd_sure 25       /* positive - negative (mark+mark_doubt)   */
                /* (min to be sure if no alternative words)*/
#define VMwrd_bad_alt_nmb  10  /* negative */
#define VMwrd_low_alt_nmb   5 /*8*/  /* negative */
#define VMwrd_voc_prob     70  /* 60*/  /* positive -- Lepik set up to 60 from 50
                * due to stdc20-8  (INMOS                */
#define VMwrd_contxt_ok     9  /* positive: context of word = cont of obj */
#define VMwrd_dif_alt_wt    6  /* negative */
#define VMwrd_repl_alt      5  /* negative: special replacement done      */
#define VMwrd_gc_alt        7  /* negative: glue-cut            done      */
#define VMwrd_change        5  /* negative: change of word      done      */
#define VMwrd_propind_pen  10  /* negative: penalty by proportion-ty index*/
#define VMwrd_doubt_pen    17  /* negative: default penalty for doubt     */
#define VMwrd_delim        30  /* negative: delimeter in a word chain     */
#define VMwrd_alts          2  /* negative: not first alts are presented  */
/* ====================================================================== */
/*                      Position evaluation multipliers:                  */
/* ====================================================================== */
#define VMpos_max_prob   50  /* positive: max prob among alts */
#define VMpos_blank      40  /* negative: it's blank          */
#define VMpos_spsp       30  /* negative:     suspected space */
#define VMpos_hyphen     30  /* negative                      */
#define VMpos_gc         10  /* negative: glue-cut possible   */
#define VMpos_alt_nmb     5  /* negative                      */
/* ====================================================================== */
/*                      Letter evaluation multipliers:                    */
/* ====================================================================== */
#define VMalt_contxt_ok     9  /* positive: context of ltr = cont of pos  */
#define VMalt_dif          15  /* negative: distance from 1-st in alt-list*/
#define VMalt_prob         40  /* positive: original probability          */
#define VMalt_probBOX      40  /* positive: probability by BOX            */
#define VMalt_repl         30  /* negative: special replacement done      */
/* ====================================================================== */

/*********************************************************/
#define R_H 48          /* raster height */
#define R_W 128         /* raster width (in dots) */
#define R_WB R_W/8      /* raster width (in bytes) */
#define R_S R_H*R_W/8   /* raster size */


/* #define WIDE_LETTER   1.90 */        /*  see spellart.c */
/* #define WIDE1_LETTER  0.55 */        /*  see spellart.c */
/* #define NARROW_LETTER 0.2  */        /*  see spellart.c */

#define WIDE_LET1 19
#define WIDE_LET2 10
#define WIDE1_LET1 55
#define WIDE1_LET2 100
#define NARROW_LET1 2
#define NARROW_LET2 10

/**********************************************************/
#define batch_run sp_batch_run
//#define list sp_list
#define tiger_dir sp_tiger_dir
//#define tableBOX sp_tableBOX

#define  STAT_ERR           0x1   /* no static vocabilary */
#define  DYN_ERR            0x2   /* cannot open a dynamic vocabilary */
#define  TWICE_ERR          0x3   /* spelling rerun on the page */
#define  MEM_ERROR          0x4   /* no enough memory */
#define  WRONG_FILE         0x5   /* incorrect ed-file */
#define   NO_PAGE           0x6   /* no ed-file */
#define  REREC_ERR          0x7   /* recognition error */
#define VOC_NOTLOADED       0x8   /* can not load one of vocabulary from list file */
#define VOC_TOOLARGELIST    0x9   /* too many vocs engaged ( more than 8) */
/************************************************************/
/****                   STATISTIC COLLECTION             ****/
/************************************************************/
typedef struct stat_of_replace
    { BYTE   was;
      BYTE    be;
      INT     no;
    } RSTAT;

#define RST_BUFF_SIZE  12

typedef struct wordstack
    { WTOKEN  * * last;
      INT          depth;
    } WSTACK;

#define MAX_WSTACK_DTH 1000

//#include "msgerr.h"

// 08-13-93 08:41pm, Mike
// 6-bit dictionary ...
#include "speltab.h"

#endif   //#ifndef __SPEL__DEFS__
