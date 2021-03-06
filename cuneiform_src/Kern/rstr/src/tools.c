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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "status.h"
#include "func.h"
#include "resource.h"
#include "ligas.h"
#include "lang.h"
#include "tuner.h"
#include "p2libr.h"
#include "ccomdefs.h"

LONG  EVNRecog_lp(/*c_comp*/CCOM_comp *ec, BYTE * lp, INT lth, BYTE *res    );
extern BYTE db_status;	// snap presence byte
extern BYTE db_trace_flag;  // 2 - more detailed estimate (ALT-F7)
extern CHAR db_pass;
extern INT nIncline;
extern void ErrorExit(int Code);
//#define NUMBER_OF_CELLS (0x30000/sizeof (cell))
extern PBYTE ED_file_bound,ED_file_end/* memory_pool_end*/;
static PBYTE kit_start, kit_curr;

static cell first_cell, last_cell;
static cell * empty_cell, * free_cell_chain, * cell_boundary;
extern c_comp wcomp;
extern version * start_rec, * rec_ptr;
extern INT lpool_lth;
extern BYTE lpool[];

static PBYTE save_kit(PBYTE c, INT l);
static void rest_kit(PBYTE s, PBYTE k);
static INT second_recog(cell *c);
static INT vers_to_cell(cell *c);
static PBYTE comp_to_kit(MN * mn);
static void comp_from_kit(cell *c);

static BYTE il1_pool[8192]={0};	// 19.07.2001 E.P.

CHAR deb_messages[]="Virtal shape set\0Skeleton match got\0"
				"Create scaled shape\0"
				"Vectorization in progress\0"
				"Pattern matching: est = %d\0"
				"Fragments separation\0"
				"Fragm. 1 match = %d\0"
				"Fragm. 2 match = %d\0"
				"Fragm. 3 match = %d\0"
				"Shape normalization\0"
				"Roughing\0"
				"Focusing = %d\0"
				"Tilting done\0"
				"Shape oscilation: %d\0"
				"Phase shift found\0";
extern BYTE work_raster_1[];


/*======================= Memory management ===========================*/
PBYTE il1_pool_ptr() { return il1_pool; }
PBYTE t_raster() { return work_raster_1; }

/*====================== Cell internal actions =======================*/

cell * cell_f() { return &first_cell; }
cell * cell_l() { return & last_cell; }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//AK debug
// ��������� ������� � �������� �� ���������
void AKCheckChain()
{
	cell * ForwardChain;
	cell * BackwardChain;
	cell * Back;
	cell * BackTwo;
	cell * Appendix = NULL;
	cell * LossCell;
	cell * Cicle;

	// ��������� ������� ������
	for ( Back = cell_f(), ForwardChain = cell_f()->next;
	      ForwardChain != cell_l() && ForwardChain->next != NULL;
		  ForwardChain = ForwardChain->next )
	{
		// ��������� �� �������������
		for ( Cicle = cell_f();
		      Cicle != ForwardChain;
			  Cicle = Cicle->next )
			if (ForwardChain->next == Cicle)
			{	//���� �������������...
				ForwardChain = NULL;
				break;
			}
		// �������� �������� �����
		if ( ForwardChain->prev != Back )
		{
			LossCell = ForwardChain->prev;
			ForwardChain->prev = Back;
		}

		Back = ForwardChain;
	}

	// ���� ������ �� �����...
	if ( ForwardChain == cell_l() )
		return;
	// ��� �� ������...
	if ( ForwardChain->prev != Back )
		ForwardChain->prev = Back;

	// ��������� ������� ������
	for ( Back = cell_l(), BackwardChain = cell_l()->prev;
	      BackwardChain != cell_f() && BackwardChain->prev != NULL;
		  BackwardChain = BackwardChain->prev )
	{
		// �������� �������� �����
		if ( BackwardChain->next != Back )
		{
			// �������, ���� ���� ��������
			for ( BackTwo = BackwardChain, Appendix = BackwardChain->next;
			      Appendix != cell_l() && Appendix->next != NULL;
				  Appendix = Appendix->next )
			{
				if ( Appendix->prev != BackTwo )
				{
					LossCell = Appendix->prev;
					Appendix->prev = BackTwo;
				}
			}
			// ���� �� ��������� ����� �� ������ �� ������, �� ���������
			if ( Appendix == ForwardChain )
			{
				ForwardChain->next = Back;
				Back->prev = ForwardChain;
				return;
			}
			// ���� ����� ��������� �� ����� �� ������...
			if ( Appendix != cell_l() )
			{
				Appendix->next = Back;
				Back->prev = Appendix;
				BackwardChain = Back;
			}
		}
		// ��������� �� ���������� next
		if ( BackwardChain->prev == ForwardChain )
		{
			ForwardChain->next = BackwardChain;
			return;
		}
		Back = BackwardChain;
	}

	// ���� �� ������ �� ������...
	if ( BackwardChain != cell_f() )
	{
		ForwardChain->next = BackwardChain;
		BackwardChain->prev = ForwardChain;
		return;
	}

	// ���� ����� ������ �� ������, � ������ ������ �� ������,
	// �� ���� ��������� �� ������, ������������ ��� � ������
	// (�� ������ ������ ��������, ������� �� �������� �����
	if ( BackwardChain == cell_f()  && ForwardChain != cell_l() )
	{
		ForwardChain->next = Back;
		Back->prev = ForwardChain;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������. ���� �� ����� cell � �������
INT AKCellInChain(cell * Cell)
{
	cell * Count;

	// ��������� ������� �� ������ ������������ ��� ������
	AKCheckChain();

	for (Count = cell_f(); Count != cell_l(); Count=Count->next)
	{
		if ( Count == Cell )
			return TRUE;
	}

	if ( Cell == cell_l() )
		return TRUE;

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void AKTryChainRecover( cell ** c )
{
	AKCheckChain();
	// ���� ������ ������� �� �������� ����������,
	// ���������, ��������� �� c � �������
	if ( (*c)->next == NULL )
	{
		// ���� ���, ��������� ��������� � ����������, ����� � ������
		if ( !AKCellInChain(*c) )
		{
			if ( AKCellInChain((*c)->prev) )
				*c = (*c)->prev;
			else
				*c = cell_f()->next;
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void AKClearVers()
{
	cell * Count;
	char   Let;
	//BYTE   Prob;
	int    i;

	// ��������� ������� �� ������ ������������ ��� ������
	AKCheckChain();

	for (Count = cell_f()->next; Count != cell_l(); Count=Count->next)
	{
		for ( i = 0; /*Count->vers[i].let != 0x0 &&*/ i < VERS_IN_CELL/*Count->nvers*/ ; i++ )
		{
			Let = Count->vers[i].let;

			if ( !proplet(Let) /*&& Let != bad_char*/ )
			{ // ���� ������ �� �� ����
				Count->vers[i].let  = 0;                 //bad_char;
				Count->vers[i].prob = 0;
			}
		}

		sort_vers(Count);
		/*
		if (Count->vers[0].let == bad_char)
		{
			Count = del_cell(Count);
		}
		*/
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void set_bad_cell(cell *c)
 {
 c->nvers=0;
 c->recsource = 0;
 c->history=0;
 c->bottom_accent =c->accent_leader = 0;
 c->vers[0].let=bad_char;
 c->vers[0].prob=0;
 c->vers[1].let=0;
 if (c->flg & (c_f_let+c_f_bad)) c->flg=c_f_bad;
 }

void sort_v_vect(INT n, version *v0)
 {
 version *v,*vs,*vm;
 BYTE l;

 for (vs=v0+1,vm=v0+n; vs<vm; vs++)
  for (v=vs; v>v0 && (v-1)->prob<v->prob; v--)
   {
   l=(v-1)->prob;
   (v-1)->prob=v->prob;
   v->prob=l;
   l=(v-1)->let;
   (v-1)->let=v->let;
   v->let=l;
   }
 }

void sort_vers(cell *c)
 {
 version *v,*vs,*vm;
 BYTE l;

 if (c->nvers <= 0)  {set_bad_cell(c); return; }
 for (vs=c->vers+1,vm=c->vers+c->nvers; vs<vm; vs++)
  for (v=vs; v>c->vers && (v-1)->prob<v->prob; v--)
   {
   l=(v-1)->prob;
   (v-1)->prob=v->prob;
   v->prob=l;
   l=(v-1)->let;
   (v-1)->let=v->let;
   v->let=l;
   }
 for (v=vm-1; v>=c->vers && !v->prob; v--,(c->nvers)--) ;
 if (!c->nvers) set_bad_cell(c);
 }

#define MAX_PROB 254

void cell_bonus(cell *C, version *pVer, INT BonVal)
{
INT resp;
version *pv;
INT wp,wp1;

 if (pVer->prob != 0)
  {
   resp = pVer->prob + BonVal;
   wp   = pVer->prob + BonVal;
   if ( wp < 0 )
     resp = 0;
   if ( wp > MAX_PROB )
    {
     wp -= MAX_PROB;
     for (pv = C->vers; pv->let != 0 ; pv++)
      {
       if ((wp1=(pv->prob - wp)) < 0)
        pv->prob = 2;
       else
        pv->prob = (BYTE)wp1;
      }
     pVer->prob = MAX_PROB;
    }
   else pVer->prob = (BYTE)resp;
  }
}

void cell_bonus_let(cell *C, CHAR Let, INT BonVal)
{
version *pv;
 for (pv=C->vers; pv->let != 0 ; pv++)
  {
  if (let_sans_acc[pv->let] == Let)
   cell_bonus(C, pv, BonVal);
  }
}
/*=================== Start of string processing ======================*/
extern BYTE *CellsPage_rstr, *CellsPageEnd_rstr;
void setup_string()
{
LONG    number_of_cells=((LONG)CellsPageEnd_rstr-(LONG)CellsPage_rstr)/sizeof (cell);
 empty_cell=(cell *) (CellsPage_rstr);
 cell_boundary = empty_cell + number_of_cells;//NUMBER_OF_CELLS;
 free_cell_chain=NULL;
//Paul 04-04-2002
   cell_f()->next   =cell_l();
   cell_f()->nextl  =cell_l();
   cell_l()->prev   =cell_f();
   cell_l()->prevl  =cell_f();
//
}
void kit_init() {
  kit_start = kit_curr = ED_file_bound; }

PBYTE give_kit_addr() { return kit_curr; }

void take_kit_addr(PBYTE a)
         { kit_curr = a;
         }

#include <assert.h>
cell * new_cell()
{
 cell * c;
 if ((c=free_cell_chain) != NULL)
	 free_cell_chain=c->next;
 else
	 c=empty_cell++;
 //assert(c < cell_boundary); // Valdemar
 if( c >= cell_boundary )
    ErrorExit (RSTR_ERR_NOPLACE);
 c->difflg = 0;
 c->bas1=c->bas2=c->bas3=c->bas4=c->basflg=0;
 c->recsource = 0;
 c->history=0;
 c->flg = 0;
 c->flg_new = 0;
 c->font = 0;
 c->n_baton=NO_BATONS;
 c->pos_inc = erect_no;
 c->accent_leader = 0;
 c->bottom_accent = 0;
 c->language = language;
 err_pnlet(c);
 err_pncell(c);
 c->clink = -1;
 c->clu_attr = 0;
 c->nClust   = 0;
 c->dup = dup_default;
 c->dupstart    =   0;
 c->dupend      =   0;
 c->dlang_dup   =   0;  // Nick 17.02.2001 - was error!
 c->cg_flag_fine = 0;
 c->pr_vers.let=bad_char;   c->pr_vers.prob=0;
 return c;
}
/////////////////////////////////////////////////////////////////////////////////////////

void free_cell(cell *c)
{
	c->next=free_cell_chain;
	err_pnlet(c);       // AL 940319
	err_pnprevcell(c);  // AL 940319
	c->flg = 0xff;      // AL 940319   0xff indicates: deleted
	free_cell_chain=c;
}
/////////////////////////////////////////////////////////////////////////////////////////
//======================= Kit function ===============================

static PBYTE save_kit(PBYTE c, INT l)
{
 PBYTE w;
 if (ED_file_end - kit_curr - 2 < l)
	ErrorExit (RSTR_ERR_NOPLACE);
 *(PINT) kit_curr=l; memcpy (kit_curr+2,c,l);
 w=kit_curr; kit_curr+=l+2;
 return w;
}

static void rest_kit(PBYTE s, PBYTE k)
{
 memcpy(s,k+2,*(PINT)k);
}

static PBYTE comp_to_kit(MN * mn)
{
 PBYTE sv;
 INT lth;
 MN_to_line(mn);
 lth = sizeof(c_comp) + lpool_lth + 2;
 wcomp.size = (lth + 15)/16; // Vald 11-24-95 05:06pm
 wcomp.lines = sizeof(c_comp);
 if (ED_file_end - kit_curr < lth)
	 ErrorExit (RSTR_ERR_NOPLACE);
 memcpy(kit_curr,&wcomp,sizeof(wcomp));
 *(INT *)(kit_curr+sizeof(wcomp)) = lpool_lth;
 memcpy(kit_curr+sizeof(c_comp)+2,lpool,lpool_lth);
 sv=kit_curr;
 kit_curr+=sizeof(c_comp)+2+lpool_lth;
 return sv;
}



c_comp * comp_vers_to_kit(MN * mn, c_comp *c)
{
 PBYTE sv=kit_curr;
 INT lth;
 MN_to_line(mn);
 if (c->size == 1)
  {
   lth=sizeof(c_comp) + lpool_lth;
   if (ED_file_end - kit_curr < lth)
	   ErrorExit (RSTR_ERR_NOPLACE);
   wcomp.type=ch_punct; wcomp.records=0; wcomp.lines=sizeof(c_comp);
   memcpy(kit_curr,&wcomp,sizeof(c_comp));
   ((c_comp*)kit_curr)->size=(lth+sizeof(lpool_lth)+15)/16; // Vald 02-15-96 07:12pm
   *(INT *)(kit_curr+sizeof(c_comp)) = lpool_lth;
   kit_curr+=sizeof(c_comp)+sizeof(INT);
   memcpy(kit_curr,lpool,lpool_lth);
   kit_curr+=lpool_lth;
   return (c_comp *)sv;
  }
 lth = sizeof(c_comp) + lpool_lth + (c->nvers+1)*sizeof(version);
 if (ED_file_end - kit_curr < lth)
	 ErrorExit (RSTR_ERR_NOPLACE);
 memcpy(kit_curr,c,sizeof(wcomp)+(c->nvers+1)*sizeof(version));
 {
  c_comp * cc = (c_comp *)kit_curr;
  cc->upper=wcomp.upper; cc->left=wcomp.left; cc->h=wcomp.h; cc->w=wcomp.w;
  cc->rw=wcomp.rw;
  cc->size=(lth+sizeof(lpool_lth)+15)/16; // Vald 11-24-95 05:09pm
  cc->records=sizeof(c_comp);
  cc->lines=lth-lpool_lth;
  cc->nl=wcomp.nl; cc->begs=wcomp.begs; cc->ends=wcomp.ends;
 }
 kit_curr+=sizeof(wcomp)+(c->nvers+1)*sizeof(version);
 *(INT *)kit_curr = lpool_lth;
 memcpy(kit_curr+sizeof(lpool_lth),lpool,lpool_lth);
 kit_curr+=sizeof(lpool_lth)+lpool_lth;
 return (c_comp *)sv;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
static void comp_from_kit(cell *c)
{
	c_comp * w = c->env;
	//AK! add c/g close bag c->evn == NULL
	if ( c->env )
	{
		memcpy (&wcomp,w,sizeof(c_comp));
		w = (c_comp *)((PBYTE)w + w->lines);
		lpool_lth=*(PINT)w;
		w=(c_comp *)((PINT)w+1);
		memcpy (lpool,w,lpool_lth);
		rec_ptr=start_rec;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void online_comp(c_comp *w)
{
 memcpy (&wcomp,w,sizeof(c_comp));
 w = (c_comp *)((PBYTE)w + w->lines);
 lpool_lth=*(PINT)w; w=(c_comp *)((PINT)w+1);
 memcpy (lpool,w,lpool_lth);
 rec_ptr=start_rec;
}

#define NCOMPMAX 8

BOOL compose_cell(INT n,cell **clist,cell *c)
 {
 INT i,top,bot,left,right,hmax,imax;
 c_comp *elist[NCOMPMAX];
 INT inc;
 BOOL   ret=TRUE;

 for ( top=left=10000,imax=hmax=bot=right=-10000,i=0; i<n && i<NCOMPMAX; i++)
  {
  if (clist[i]->r_row<top)
   top=clist[i]->r_row;
  if (clist[i]->r_row+clist[i]->h>bot)
   bot=clist[i]->r_row+clist[i]->h;
  if (clist[i]->r_col<left)
   left=clist[i]->r_col;
  if (clist[i]->r_col+clist[i]->w>right)
   right=clist[i]->r_col+clist[i]->w;
  if( hmax<clist[i]->h )
    {
    hmax=clist[i]->h;
    imax=i;
    }
  elist[i]=clist[i]->env;
  }
  if( bot-top>RASTER_MAX_HEIGHT )
    ret=FALSE;
  if( right-left>RASTER_MAX_WIDTH )
    ret=FALSE;
//if( !ret )
//    return ret;
 if( ret )
     {
     inc = erection_compose_inc(n, clist);
     if( inc!=NO_INCLINE )
        {
        c->stick_inc = inc;
        c->pos_inc   = erect_rot;
        }
     c->r_row=top;
     c->h=bot-top;
     c->r_col=left;
     c->w=right-left;
     c->row=c->r_row-(INT)((LONG)nIncline*c->r_col/2048);
     c->col=c->r_col+(INT)((LONG)nIncline*c->r_row/2048);
     c->env=compose_comp(i,elist);
     c->cg_flag|=c_cg_comp;
     }
 else if( imax!=-1 && c!=clist[imax] )
    {
    cell *n=c->next,*p=c->prev,*nl=c->nextl,*pl=c->prevl;
    WORD    flg=c->flg;
    *c=*clist[imax];
    c->next=n;
    c->prev=p;
    c->nextl=nl;
    c->prevl=pl;
    c->flg=flg;
    }
 for (n=i,i=0; i<n; i++)
      if (clist[i]!=c)
       del_cell(clist[i]);
 return ret;
 }

// analog of compose_cell whithout deleting
BOOL compose_cell_save(INT n,cell **clist,cell *c)
 {
 INT i,top,bot,left,right,hmax,imax;
 c_comp *elist[NCOMPMAX];
 INT inc;
 BOOL   ret=TRUE;

 for (top=left=10000,imax=hmax=bot=right=-10000,i=0; i<n && i<NCOMPMAX; i++)
  {
  if (clist[i]->r_row<top)
   top=clist[i]->r_row;
  if (clist[i]->r_row+clist[i]->h>bot)
   bot=clist[i]->r_row+clist[i]->h;
  if (clist[i]->r_col<left)
   left=clist[i]->r_col;
  if (clist[i]->r_col+clist[i]->w>right)
   right=clist[i]->r_col+clist[i]->w;
  elist[i]=clist[i]->env;
  if( hmax<clist[i]->h )
    {
    hmax=clist[i]->h;
    imax=i;
    }
  }
 if( bot-top>RASTER_MAX_HEIGHT )
    ret = FALSE;
  if( right-left>RASTER_MAX_WIDTH )
    ret = FALSE;
//if( !ret )
//    return ret;
 if( ret )
     {
     inc = erection_compose_inc(n, clist);
     if( inc!=NO_INCLINE )
        {
        c->stick_inc = inc;
        c->pos_inc   = erect_rot;
        }
     c->r_row=top;
     c->h=bot-top;
     c->r_col=left;
     c->w=right-left;
     c->row=c->r_row-(INT)((LONG)nIncline*c->r_col/2048);
     c->col=c->r_col+(INT)((LONG)nIncline*c->r_row/2048);
     c->env=compose_comp(i,elist);
     c->cg_flag|=c_cg_comp;
     }
  else if( imax!=-1 && c!=clist[imax] )
    {
    cell *n=c->next,*p=c->prev,*nl=c->nextl,*pl=c->prevl;
    WORD    flg=c->flg;
    *c=*clist[imax];
    c->next=n;
    c->prev=p;
    c->nextl=nl;
    c->prevl=pl;
    c->flg=flg;
    }
 return ret;
 }


c_comp *compose_comp(INT n,c_comp **c)
 {
 INT lower,right,i,j,l,nl,du,dl,lth;
 PBYTE w,sv;
 lnhead *ln;
 interval *intv;

 memcpy(&wcomp,c[0],sizeof(c_comp));
 wcomp.lines=sizeof(c_comp);
 wcomp.nl=0;
 lower=wcomp.upper+wcomp.h;
 right=wcomp.left+wcomp.w;

 for (i=0; i<n; i++)
  if (c[i]!=NULL)
   {
   if (c[i]->upper<wcomp.upper)
    wcomp.upper=c[i]->upper;
   if (c[i]->upper+c[i]->h>lower)
    lower=c[i]->upper+c[i]->h;
   if (c[i]->left<wcomp.left)
    wcomp.left=c[i]->left;
   if (c[i]->left+c[i]->w>right)
    right=c[i]->left+c[i]->w;
   wcomp.nl+=c[i]->nl;
   }
   /*
 if( lower-wcomp.upper>RASTER_MAX_HEIGHT )
    return NULL;
 if( right-wcomp.left>RASTER_MAX_WIDTH )
    return NULL;
    */
 wcomp.h=lower-wcomp.upper;
 wcomp.w=right-wcomp.left;
 wcomp.rw=(wcomp.w+7)/8;

 lpool_lth=2;
 for (l=2,i=0; i<n; i++)
 {
  if (c[i]!=NULL)
   {
   w=(PBYTE)(c[i])+c[i]->lines;
   l+=*((INT *)w)-((i==n-1)?0:2);
  }
 }
 if( l>LPOOL_SIZE)
	return NULL;
 for (i=0; i<n; i++,lpool_lth+=l)
  if (c[i]!=NULL)
   {
   w=(PBYTE)(c[i])+c[i]->lines;
   nl=c[i]->nl;
   du=c[i]->upper-wcomp.upper;
   dl=c[i]->left-wcomp.left;
   l=*((INT *)w)-((i==n-1)?0:2);
   memcpy((PBYTE)lpool+lpool_lth,w+2,l);
   for (ln=(lnhead*)((PBYTE)lpool+lpool_lth),j=0; j<nl;
					 j++,ln=(lnhead *)((PBYTE)ln+lth))
    {
    ln->row+=du;
    lth=ln->lth;
    if (!j)
     ln->flg|=l_cbeg;
    if (j==nl-1)
     ln->flg|=l_cend;
    for (intv=(interval *)((PBYTE)ln+sizeof(lnhead)); intv->l; intv++)
     intv->e+=dl;
    }
   }

 *((INT *)lpool)=lpool_lth-2;
 if( (PBYTE)(ED_file_end - kit_curr)<(PBYTE)(lpool_lth+sizeof(c_comp)) )
  ErrorExit (RSTR_ERR_NOPLACE);
 sv=kit_curr;
 wcomp.size = (sizeof(c_comp) + lpool_lth + 15) / 16; // Vald
 memcpy(kit_curr,&wcomp,sizeof(c_comp));
 memcpy(kit_curr+sizeof(c_comp),lpool,lpool_lth);
 kit_curr+=wcomp.size*16; //sizeof(c_comp)+lpool_lth; Vald 06-22-95 04:53pm
 return (c_comp *)sv;
 }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create/Delete cells
#define NoNextPrev ((cell *)0xffff0000)
void del_retain_cell(cell *c)
{
 //  AFTER TESTS PROTECT MYSELF: if (c->prev == (cell*)(0xffff0000)) return;
	//AK add c/g

	// 17.06.2002 E.P.
	if (c->prev == NoNextPrev)
		return;

	if ( c->next )
	{
		(c->prev)->next=c->next;
		(c->next)->prev=c->prev;

		if ( ( c->flg&(c_f_let|c_f_bad) && !(c->flg&(c_f_dust|c_f_punct)) ) ||
			  c->nextl && c->nextl != NoNextPrev &&
			  c->prevl && c->prevl != NoNextPrev
		   )
		{
			(c->prevl)->nextl=c->nextl;
			(c->nextl)->prevl=c->prevl;
		}
	}

	err_pnlet(c);  // AL 940319
	err_pncell(c);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AK add c/g gor cells
cell * del_cell(cell *c)
{
 cell *cc;
 cc = c->prev;
 // AK add c/g
 if ( !(c->next) )
	AKCheckChain();

 del_retain_cell(c);
 free_cell(c);
 return cc;
}


PBYTE del_save_cell(cell *c)
{
 PBYTE p;
 p =  save_kit((PBYTE)c,sizeof(cell));
 del_cell(c);
 return p;
}

cell * rest_cell(void *k, cell *ci)
{
 cell * c = new_cell();
 rest_kit((PBYTE) c, k);
 insert_cell(c,ci);
 return c;
}

void insert_cell(cell *c,cell *ci)
{
	INT col=c->col;
	if(!(ci->flg & c_f_space)) // Valdemar 02-15-96 00:17am
// Paul 10-11-96
/*
 if (col <= ci->col)  while ((ci->prev)->col > col) ci=ci->prev;
 else                 while ((ci=ci->next)->col <= col);
*/
	//AK! Crashed line when ci->prev pointed to first_cell or ci
	//    pointed to last_cell
	if (col <= ci->col)
		while ( (ci->prev && ci->prev != cell_f()) && ((ci->prev)->flg & c_f_space || (ci->prev)->col > col))
		{
			if ( ci->prev )
				ci=ci->prev;
			else
				AKCheckChain();
		}
	else
		while ((ci != cell_l() && ci->next) && (ci->flg & c_f_space || ci->col <= col))
		{
			if ( ci->next )
				ci=ci->next;
			else
				AKCheckChain();
		}
//

	(ci->prev)->next=c;
	c->prev=ci->prev;
	c->next=ci;
	ci->prev=c;

	if (c->flg & (c_f_let+c_f_bad) && !(c->flg & c_f_dust) )
	{
		ci=c->next;
		while (!(ci->flg & (c_f_fict+c_f_let+c_f_bad)) || ci->flg & c_f_dust)
			ci=ci->next;
		(ci->prevl)->nextl=c;
		c->prevl=ci->prevl;
		c->nextl=ci;
		ci->prevl=c;
	}
	else  // AL 940318
		err_pnlet (c);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// crash attention!
void insert_cell1(cell *c,cell *ci)
{
	INT col=c->col;

	if( !(ci->flg & c_f_space)) // Valdemar 02-15-96 00:17am
		if ( col <= ci->col)
			while ( (ci->prev)->col >= col )
				ci=ci->prev;
		else
			while ( (ci=ci->next)->col < col);

	(ci->prev)->next=c;
	c->prev=ci->prev;
	c->next=ci;
	ci->prev=c;

	if (c->flg & (c_f_let+c_f_bad) && !(c->flg & c_f_dust) )
	{
		ci=c->next;
		while ( !(ci->flg & (c_f_fict+c_f_let+c_f_bad)) || ci->flg & c_f_dust )
			ci=ci->next;
		(ci->prevl)->nextl=c;
		c->prevl=ci->prevl;
		c->nextl=ci;
		ci->prevl=c;
	}
	else  // AL 940318
		err_pnlet (c);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void adjust_links (cell *c)
{
 cell *cn;
 cell *cp = c;
 if ((c->flg & (c_f_bad | c_f_let)) == 0)
   return;
 if (c->flg & c_f_fict)
   return;
 while (1)
 { cp = cp->prev;
   if (cp->flg & (c_f_bad | c_f_let | c_f_fict))
   break;
 }
 if ((cn=cp->nextl) == c)
   return;
 c->nextl = cn;
 c->prevl = cp;
 cp->nextl = c;
 cn->prevl = c;
}

cell * create_cell(MN * mn, cell * ci, CHAR bdiff, CHAR dflag)
{
 cell * c = new_cell();
 accept_cell (c,(c_comp *)comp_to_kit(mn));
 c->basflg=c->bas1=c->bas2=c->bas3=c->bas4=0;
 c->difflg=dflag;
 c->bdiff=bdiff; c->reasno=0; c->cg_flag=0; c->dens=255;
 if (if_dust(c) & 0x0c)   c->flg=c_f_dust;
 else			c->flg=c_f_bad;
 set_bad_cell(c);
 c->pos_inc=erect_no;
 c->accent_leader = 0;
 c->bottom_accent = 0;
 c->n_baton=NO_BATONS;
 c->flg_new = 0;
 c->save_stick_inc=c->stick_inc=NO_INCLINE;
 c->cg_flag_fine=0;
 c->dupstart    =   0;
 c->dupend      =   0;
 c->dlang_dup   =   0;  // Nick 17.02.2001 - was error!
 insert_cell(c,ci);
 return c;
}

cell * create_cell1(MN * mn, cell * ci, CHAR bdiff, CHAR dflag)
{
 cell * c = new_cell();
 accept_cell (c,(c_comp *)comp_to_kit(mn));
 c->basflg=c->bas1=c->bas2=c->bas3=c->bas4=0;
 c->difflg=dflag;
 c->bdiff=bdiff; c->reasno=0; c->cg_flag=0; c->dens=255;
 if (if_dust(c) & 0x0c)   c->flg=c_f_dust;
 else			c->flg=c_f_bad;
 set_bad_cell(c);
 c->pos_inc=erect_no;
 c->accent_leader = 0;
 c->bottom_accent = 0;
 c->n_baton=NO_BATONS;
 c->flg_new = 0;
 c->save_stick_inc=c->stick_inc=NO_INCLINE;
 c->cg_flag_fine=0;
 c->dupstart    =   0;
 c->dupend      =   0;
 c->dlang_dup   =   0;  // Nick 17.02.2001 - was error!
 insert_cell1(c,ci);
 return c;
}

cell * create_cell_work(MN * mn, cell * ci, CHAR bdiff, CHAR dflag)
{
 cell * c = new_cell();
 accept_cell (c,(c_comp *)comp_to_kit(mn));
 c->basflg=c->bas1=c->bas2=c->bas3=c->bas4=0;
 c->difflg=dflag;
 c->bdiff=bdiff; c->reasno=0; c->cg_flag=0; c->dens=255;
 if (if_dust(c) & 0x0c)   c->flg=c_f_dust;
 else			c->flg=c_f_bad;
 set_bad_cell(c);
 c->col        += ci->col;
 c->r_col      += ci->r_col;
 c->row        += ci->row;
 c->r_row      += ci->r_row;
 c->env->upper += ci->r_row;
 c->env->left  += ci->r_col;
 if( ci->pos_inc&erect_rot )
   {
   c->pos_inc   = erect_rot;
   c->stick_inc = ci->stick_inc;
   }
 else
   {
   c->stick_inc = NO_INCLINE;
   c->pos_inc   = erect_no;
   }
 insert_cell(c,ci);
 return c;
}

/*=============== Cell rocognition ===========================*/

static INT vers_to_cell(cell *c)
{
 INT i;
 c->recsource = 0;
 if ((i=rec_ptr-start_rec) !=0)
  {
   memcpy(c->vers,start_rec,i*sizeof(version)); c->nvers=i;
   c->vers[i].let=0;
   c->flg = c_f_let;
   c->recsource |= c_rs_ev;
   c->history   |= c_rs_ev;
   return i;

  }
 set_bad_cell(c); return 0;
}

static INT second_recog(cell *c)
{
 levcut(c,1);
 if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,c,"second rec linear");
  criteria(c);
 if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,c,"second rec criteria");
 if ( c->nvers == 0 )  {
    c->recsource &= 0;
    c->history   &= 0;
    }
 return c->nvers;
}


INT short_recog_cell (cell *c)
{
 // was && �� ���� ��� c->evn - ��� ������������?
 if( (c->cg_flag & c_cg_comp) ||
     (!c->env) )
   return 0;

 comp_from_kit(c);
 {
INT n,i;
BYTE res[20];
CCOM_comp cc;
//������ � ��������� CCOM_comp ������ ��� EVNRecog_lp
cc.h = c->env->h;
cc.w = c->env->w;
cc.rw = c->env->rw;
cc.nl = c->env->nl;
cc.begs = c->env->begs;
cc.ends = c->env->ends;
cc.scale = c->env->scale;

// n = (INT)EVNRecog_lp(c->env,lpool,lpool_lth,&res[0]);
 n = (INT)EVNRecog_lp(&cc,lpool,lpool_lth,&res[0]);

//�������������� �� ��������� CCOM_comp ����������� EVNRecog_lp
 c->env->type = cc.type;
 c->env->cs = cc.cs;
 c->env->pidx = cc.pidx;
 c->env->reasno = cc.reasno;

 if( n )
     {
     for(i=0;i<n;i++)
	    {
	    c->vers[i].let=res[i];
	    c->vers[i].prob=254;
	    }
       c->vers[n].let=0;
       c->flg = c_f_let;
       c->recsource |= c_rs_ev;
       c->history   |= c_rs_ev;
     }
  else
    {
    c->vers[0].let=176;
    c->vers[1].let=0;
    c->flg = c_f_bad;
    }
  c->nvers=n;
}

 //recog_letter();
 v2_pidx_crit(c);
 sort_vers(c);
 //c->reasno = wcomp.reasno;
 //return vers_to_cell(c);
 return c->nvers;
}

INT recog_cell(cell *c)
{
if( !c->env ) return 0; // Oleg : can't recog null cells
 short_recog_cell(c);
 if(pass4_in) // Dangerous
 { c->nvers=c->vers[0].let=c->vers[0].prob=0; } // kill events
 if(language == LANG_RUSSIAN) r_criteria(c,NULL);
 return second_recog(c);
}

INT reco2_cell(cell *c)
{
 c->reasno=0;
 second_recog(c);
 if ((db_pidx_crit) && c->nvers)
 {
   v2_pidx_crit(c);
   if (db_status && (db_trace_flag & 2))
     est_snap(db_pass,c,"second rec proport");
 }
 if ( c->nvers == 0 )  {
    c->recsource = 0;
    c->history   = 0;
    }
 return c->nvers;
}

INT recop_cell(cell *c)
{
 c->reasno=0; criteria(c);  if (db_pidx_crit) v2_pidx_crit(c);
 if ( c->nvers == 0 )  {
    c->recsource = 0;
    c->history   = 0;
    }
 return c->nvers;
}

/*================ Make raster function =======================*/
PBYTE save_raster(cell *c)
{
 comp_from_kit(c); return make_raster();
}

PBYTE save_raster_align8(cell *c)
{
 comp_from_kit(c);
 wcomp.rw = ((wcomp.w+63)/64)*8;
 return make_raster();
}

void add_vers(cell *bc, version *wv)
{
 INT nv;
 nv=bc->nvers;
 if (nv == VERS_IN_CELL-1)
  {
   nv--;
   if (bc->vers[nv].prob >= wv->prob) return;
   bc->vers[nv].let=wv->let;
   bc->vers[nv].prob=wv->prob;
   sort_vers(bc);
  }
 else
  {
   bc->vers[bc->nvers].let=wv->let;
   bc->vers[bc->nvers].prob=wv->prob;
   bc->nvers++;
   sort_vers(bc);
  }
}

BOOL    comp_versions(version *v, version *w, INT n,INT    snvers)
{
INT i;

if( n!=snvers )
    return TRUE;
for(i=0;i<n;i++)
    {
    if( v[i].let!=w[i].let || v[i].prob!=w[i].prob )
        return TRUE;
    }
return FALSE;
}

void del_version(  cell *c,BYTE let)
{
 INT i,nvers;
 version *v ;

 if( c->nvers<1 )
  return;

 v     = c->vers;
 nvers = c->nvers;

 for(i=0;i<nvers;i++)
  if( v[i].let==let )
    {
    for(i++;i<nvers;i++)
      v[i-1]=v[i];
    v[i].let=v[i].prob=0;
    c->nvers--;
    return;
    }
return;
}

#define s_ans(a) { rec_ptr->let = a; rec_ptr->prob = 254; rec_ptr++; }
INT stick_w_4()
{
 INT answ;
 answ = typ_thin_stick((lnhead *)lpool,wcomp.h,wcomp.w);
 rec_ptr = start_rec;
 if (answ == 0) goto ret;
 s_ans('!');
 if ((answ & 0xC0) == 0x80)   s_ans('1');
 if (language == LANG_RUSSIAN ){ s_ans('|'); goto ret; }
 if ((answ & 0x03) == 0x02)   s_ans('f');
 if ((answ & 0x0C) == 0x08)   s_ans('r');
 if ((answ & 0x30) == 0x20)   s_ans('t');

 if (language == LANG_POLISH )
	if ((answ & 0x300)== 0x300)  s_ans(POLISH_l);

 if (rec_ptr - start_rec < 7) 	s_ans('j');
 s_ans('l'); s_ans('i'); s_ans('I');
ret:
 return (wcomp.nvers = rec_ptr - start_rec);
}
