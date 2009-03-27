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

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "cstr.h"
#include "ccom.h"
//#include "exc.h"
#include "resource.h"

static  CSTR_head head, tail;
static  Int32     num_lines      = 0,num_fragments0=0,num_fragments1 = 0;
static  Int32     user_number = 0xFFFFFF+1;
Word16           wHeightRC      = 0;
Word16           wLowRC         = CSTR_ERR_NO;
static  CSTR_line *FragmFirst0=NULL, *FragmLast0=NULL;
static  CSTR_line *FragmFirst1=NULL, *FragmLast1=NULL;
static  Int32 FragmMin[2], FragmMax[2];
//int     mem=0;
// memory funct
static void *   cstr_alloc(Word32 len)
    {
    void *ma = malloc(len);
    if( !ma )
        return NULL;
    memset(ma,0,len);
    //mem+=len;
    return ma;
    }
static void     cstr_free(void *ptr,Word32 len) { free(ptr);};
static void *   cstr_realloc(void *ptr,Word32 len) { return realloc(ptr,len);};
static void * (*my_alloc)(Word32 len)=cstr_alloc;
static void   (*my_free)(void *,Word32 len)=cstr_free;
static void * (*my_realloc)(void *,Word32 len)=cstr_realloc;
static Bool32 RecRaster2rst(RecRaster *recr,CSTR_cell  *cell);
/////////////////////
// common functions
/////////////////////
CSTR_FUNC(Bool32)  CSTR_Init( Word16 wHeightCode , Handle hStorage)
{
wHeightRC = wHeightCode;
wLowRC=CSTR_ERR_NO;
num_lines=0;
// setup first & last heads
head.Properties = CSTR_LN_FICT;
tail.Properties = CSTR_LN_FICT;
head.next       = &tail;
tail.prev       = &head;
user_number = 0xFFFFFF+1;
FragmFirst0=my_alloc(8000*sizeof(CSTR_line));
if( !FragmFirst0 )
    {
    wLowRC=CSTR_ERR_NOMEMORY;
    return FALSE;
    }
FragmLast0 =my_alloc(8000*sizeof(CSTR_line));
if( !FragmLast0 )
    {
    wLowRC=CSTR_ERR_NOMEMORY;
    return FALSE;
    }
FragmFirst1=my_alloc(8000*sizeof(CSTR_line));
if( !FragmFirst1 )
    {
    wLowRC=CSTR_ERR_NOMEMORY;
    return FALSE;
    }
FragmLast1 =my_alloc(8000*sizeof(CSTR_line));
if( !FragmLast1 )
    {
    wLowRC=CSTR_ERR_NOMEMORY;
    return FALSE;
    }
FragmMin[0]=FragmMin[1]=16000;
FragmMax[0]=FragmMax[1]=-1;
return TRUE;
}


CSTR_FUNC(void)    CSTR_Done(void)
{
CSTR_head       *line=head.next, *nline;
// delete all heads
for( ;line && line!=&tail;  )
    {
    nline   =   line->next;
    CSTR_DeleteLine ((CSTR_line)line);
    line    =   nline;
    }

wLowRC = CSTR_ERR_NO;
wHeightRC=0;
num_lines = 0;
user_number = 0xFFFFFF+1;
my_free(FragmFirst0,0);
my_free(FragmLast0,0);
my_free(FragmFirst1,0);
my_free(FragmLast1,0);
FragmMin[0]=FragmMin[1]=16000;
FragmMax[0]=FragmMax[1]=-1;
return;
}


CSTR_FUNC(Word32)   CSTR_GetReturnCode(void)
{
if( wLowRC == CSTR_ERR_NO )
    return 0;
return (wHeightRC<<16)|(wLowRC-CSTR_ERR_MIN);
}

CSTR_FUNC(char*)   CSTR_GetReturnString(Word32 dwError)
{
  Word16 rc = (Word16)(dwError & 0xFFFF + CSTR_ERR_MIN);
        static char szBuffer[512];

        if( dwError >> 16 != wHeightRC)
    wLowRC = CSTR_ERR_NOTIMPLEMENT;

  if( rc > 0 && rc <= CSTR_ERR_MAX-CSTR_ERR_MIN  )
    strcpy((char *)szBuffer,CSTR_error_name [rc]);
        else
                return NULL;

        return szBuffer;
}

//////////////////////////////////
// alloc / free & access lines to
//////////////////////////////////
CSTR_FUNC(CSTR_line)    CSTR_NewLine(Int32  lineno, Int32 version,
    Int32 container)
{
CSTR_head       *line=head.next, *lineins=0, *prev, *next;
// scan all heads
if( !line )
        {
        wLowRC = CSTR_ERR_NULL;
    return (CSTR_line)0;
        }

for( ;line!=&tail; line=line->next )
    {
    if(line->number==lineno  )
        {
        if( line->version==version )
            {
            wLowRC = CSTR_ERR_DOUBLICATE;
            return (CSTR_line)0;
            }
        else
            lineins = line;
        }
    }
if( lineins == (CSTR_line)0 )
    lineins = tail.prev;

line = my_alloc(sizeof(CSTR_head));

if( !line )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_line)0;
    }
next       = lineins->next ;
prev       = lineins;
next->prev = line;
prev->next = line;
line->prev = prev;
line->next = next;

line->first.line_no = (CSTR_line)line ;
line->last .line_no = (CSTR_line)line ;
line->first.attr.flg= CSTR_f_fict;
line->first.attr.col=-16000;
line->last .attr.flg= CSTR_f_fict;
line->last .attr.col=0x7fff;
line->first.next    = &line->last;
line->last .prev    = &line->first;
line->number        = lineno;
line->version       = version;
if( container==-1 )
    {
    line->private_container = TRUE;
    line->container         = CCOM_CreateContainer();
    }
else
    {
    line->private_container = FALSE;
    line->container         =  (CCOM_handle)container;
    }

if( lineno>num_lines && lineno<10000 )
    num_lines = lineno;
return (CSTR_line)line;
}



CSTR_FUNC(CSTR_line)    CSTR_NextLine(CSTR_line start, Int32 version)
{
CSTR_head       *line=(CSTR_head        *)start;

for(line=line->next; line!=&tail && line->version!=version; line=line->next);

return line!=&tail?(CSTR_line)line:0;
}

CSTR_FUNC(CSTR_line)    CSTR_FirstLine( Int32 version)
{
return CSTR_NextLine((CSTR_line)&head, version);
}

Bool32 cstr_copy_branch(CSTR_rast sta,CSTR_rast sto, CSTR_rast be, CSTR_rast en)
{
CSTR_rast       rn, c, e=CSTR_GetNextRaster(en,255), newdn, dn;
CSTR_rast_attr  attr;
RecRaster       rs;
UniVersions     vr;
CCOM_comp    *  comp;

for(c=be;c&&c!=e;c=CSTR_GetNext(c))
    {
    if( CSTR_GetAttr ((CSTR_rast)c, &attr) &&
        CSTR_GetImage ((CSTR_rast)c, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) &&
        CSTR_GetCollectionUni((CSTR_rast)c,&vr) &&
        (comp=CSTR_GetComp(c))!=NULL )
        {
        if( c==be )
            { // first branch raster
            if( !(rn=CSTR_InsertRasterDown (sta,sto)) )
                return FALSE;
            }
        else
            {
            if( !(rn=CSTR_InsertRaster (rn)) )
                return FALSE;
            }
        if( !CSTR_SetAttr (rn, &attr) )
            return FALSE;
        if( !CSTR_StoreRaster (rn, &rs) )
            return FALSE;
        if( !CSTR_StoreCollectionUni (rn, &vr) )
            return FALSE;
        if( !CSTR_StoreScale(rn,comp->scale) )
            return FALSE;
        if( (CSTR_cell*)c->next_down )
            { //  start of bracnh
            newdn=rn;
            dn=c;
            }
        if( (CSTR_cell*)c->prev_down )
            { // end of branch
            cstr_copy_branch(dn,c,newdn,rn); // recursive processing sub-branch
            }
        }
    }

return TRUE;
}

CSTR_FUNC(Bool32)       CSTR_CopyLine(CSTR_line trg,CSTR_line   src)
{
CSTR_rast       start    = CSTR_GetFirstRaster (src), stop = CSTR_GetLastRaster (src), c;
CSTR_rast       start_new= CSTR_GetFirstRaster (trg), stop_new = CSTR_GetLastRaster (trg), cnew;
CSTR_rast_attr  attr;
RecRaster       rs;
UniVersions     vr;
CCOM_comp    *  comp;
CSTR_attr       lattr;
CSTR_rast       newdn,dn;

if( !start || !stop || !start_new || !stop_new)
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

CSTR_GetLineAttr(src,&lattr);
CSTR_SetLineAttr(trg,&lattr);
for(c=CSTR_GetNextRaster (start,CSTR_f_all); c && c!=stop; c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    if( CSTR_GetAttr (c, &attr) &&
        CSTR_GetImage (c, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) &&
        CSTR_GetCollectionUni(c,&vr) &&
        (comp=CSTR_GetComp(c))!=NULL )
        {
        if( !(cnew=CSTR_NewRaster (trg ,attr.col, attr.row, attr.w)) )
            return FALSE;
        if( !CSTR_SetAttr (cnew, &attr) )
            return FALSE;
        if( !CSTR_StoreRaster (cnew, &rs) )
            return FALSE;
        if( !CSTR_StoreCollectionUni (cnew, &vr) )
            return FALSE;
        if( !CSTR_StoreScale(cnew,comp->scale) )
         return FALSE;
        }

    if( (CSTR_cell*)c->next_down )
        { //  start of bracnh
        newdn=cnew;
        dn=c;
        }
    if( (CSTR_cell*)c->prev_down )
        { // end of branch
        cstr_copy_branch(dn,c,newdn,cnew);
        }

    }


return TRUE;
}

CSTR_FUNC(Bool32)      CSTR_ReplaceWord(CSTR_rast * dest_begin, CSTR_rast * dest_end, CSTR_rast src_begin, CSTR_rast src_end)
{
CSTR_rast       start   = src_begin, stop = src_end, c;
CSTR_rast_attr  attr;
CSTR_rast cnew;
RecRaster       rs;
UniVersions     vr;
CCOM_comp    *  comp;
CSTR_attr       lattr;
CSTR_rast       newdn,dn;
CSTR_rast newbeg = 0, newend;

if ((*dest_begin) != (*dest_end))
{
	c=CSTR_GetNextRaster (*dest_begin, CSTR_f_all);
	while ((c != (*dest_end))&&c) 
		c = CSTR_DelRaster(c);
	if (c)
		CSTR_DelRaster(c);
}
cnew = (*dest_begin);
for(c=start; c && (c!=stop); c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    if( CSTR_GetAttr (c, &attr) 
	&& CSTR_GetImage (c, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) 
	&& CSTR_GetCollectionUni(c,&vr) 
	&&(comp=CSTR_GetComp(c))!=NULL )
        {
        if( !(cnew=CSTR_InsertRaster (cnew)))
            return FALSE;
	if (!newbeg) newbeg = cnew;
	newend = cnew;
        if( !CSTR_SetAttr (cnew, &attr) )
            return FALSE;
        if( !CSTR_StoreRaster (cnew, &rs) )
            return FALSE;
        if( !CSTR_StoreCollectionUni (cnew, &vr) )
            return FALSE;
        if( !CSTR_StoreScale(cnew,comp->scale) )
         return FALSE;
        }
    
    if( (CSTR_cell*)c->next_down )
        { //  start of bracnh
        newdn=cnew;
        dn=c;
        }
    if( (CSTR_cell*)c->prev_down )
        { // end of branch
        cstr_copy_branch(dn,c,newdn,cnew);
        }
    }
 
    if (c) 
    {
	if( CSTR_GetAttr (c, &attr) 
		&& CSTR_GetImage (c, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) 
		&& CSTR_GetCollectionUni(c,&vr) 
		&&(comp=CSTR_GetComp(c))!=NULL )
	{
		if( !(cnew=CSTR_InsertRaster (cnew)))
			return FALSE;
		if (!newbeg) newbeg = cnew;
		newend = cnew;
		if( !CSTR_SetAttr (cnew, &attr) )
			return FALSE;
		if( !CSTR_StoreRaster (cnew, &rs) )
			return FALSE;
		if( !CSTR_StoreCollectionUni (cnew, &vr) )
			return FALSE;
		if( !CSTR_StoreScale(cnew,comp->scale) )
			return FALSE;
	}
	if( (CSTR_cell*)c->next_down )
        { //  start of bracnh
		newdn=cnew;
		dn=c;
        }
	if( (CSTR_cell*)c->prev_down )
        { // end of branch
		cstr_copy_branch(dn,c,newdn,cnew);
        }
    }

    CSTR_DelRaster(*dest_begin);
    *dest_begin = newbeg;
    *dest_end = newend;
    return TRUE;
}


Bool32 cstr_delete_branch(CSTR_cell   *cc)
{
CSTR_cell   *c, *e;
for(c=cc;c;c=c->next)
    {
    if( c->next_down )
        {
        cstr_delete_branch(c->next_down); // recursive processing sub-branch
        }
    if( c->next_up )
        break;
    }
e = c;
for(c=cc;c && c!=e;)
    {
    c = CSTR_DelRaster (c);
    }
CSTR_DelRaster (e);
return TRUE;
}

CSTR_FUNC(Bool32)                   CSTR_DeleteLoops(CSTR_line  lin)
{
CSTR_rast start = CSTR_GetFirstRaster (lin),stop = CSTR_GetLastRaster (lin), c;
CSTR_cell   *cc;
if( !start || !stop )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

for(c=CSTR_GetNextRaster (start,CSTR_f_all); c && c!=stop; c=CSTR_GetNext(c))
    {
    cc=(CSTR_cell*)c;
    if( cc->next_down  )
        {
        cstr_delete_branch(cc->next_down);
        }
    }

return FALSE;
}

CSTR_FUNC(Bool32)                   CSTR_DeleteLine (CSTR_line  lin)
{
CSTR_rast start = CSTR_GetFirstRaster (lin),stop = CSTR_GetLastRaster (lin), c;
CSTR_cell   *cc;
CSTR_head *line = (CSTR_head *)lin, *prev, *next;

if( !start || !stop )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

for(c=CSTR_GetNextRaster (start,CSTR_f_all); c && c!=stop; )
    {
    cc=(CSTR_cell*)c;
    if( cc->next_down  )
        {
        cstr_delete_branch(cc->next_down);
        }
    c = CSTR_DelRaster (c);
    }

if( line->private_container )
    CCOM_DeleteContainer(line->container);
prev = line->prev;
next = line->next;
prev->next = next;
next->prev = prev;
my_free(line,0);
return FALSE;
}

CSTR_FUNC(Bool32)                   CSTR_GetLineAttr (CSTR_line  linel,CSTR_attr * attr)
{
CSTR_head *line = (CSTR_head *)linel;
if( !attr || !line)
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
if( line==(CSTR_head *)0 )
    {
    wLowRC=CSTR_ERR_VALUE       ;
    return FALSE;
    }
*attr = line->attr;
attr->number  =   line->number            ;
attr->version =   line->version           ;
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_SetNewLineNumber (CSTR_line  linel, int new_number)
{
	CSTR_head *line = (CSTR_head *)linel;
	line->number = new_number;
	return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_SetLineAttr (CSTR_line      linel,CSTR_attr * attr)
{
CSTR_head *line = (CSTR_head *)linel;
if( !attr )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
if( line==(CSTR_head *)0 )
    {
    wLowRC=CSTR_ERR_VALUE       ;
    return FALSE;
    }
line->attr = *attr;
return TRUE;
}

static Bool32 cstr_CCOM2raster(CCOM_comp *comp, CSTR_cell *cell)
{
Int32       len;
RecRaster   rs;

rs.lnPixWidth   = comp->w;
rs.lnPixHeight  = comp->h;
if( comp->scale )
    {
    rs.lnPixWidth  =(rs.lnPixWidth +(1<<comp->scale)-1)>> comp->scale;
    rs.lnPixHeight =(rs.lnPixHeight+(1<<comp->scale)-1)>> comp->scale;
    }
len = REC_GW_WORD8(rs.lnPixWidth)*rs.lnPixHeight;
memset(rs.Raster,0,len);
if( !CCOM_AddLPToRaster  (comp, &rs) )
    {
    wLowRC=CSTR_ERR_TORASTER;
    return FALSE;
    }
RecRaster2rst(&rs,cell);
return TRUE;
}

static Bool32 cstr_delete_raster(CSTR_cell *cell)
{
if( cell->recRaster )
    {
    my_free(cell->recRaster,0);
    cell->recRaster=NULL;
    }
return TRUE;
}

CSTR_FUNC(CSTR_line )   CSTR_GetLineHandle ( Int32 line_no, Int32 version)
{
CSTR_head *line = head.next;

for(; line!=&tail; line=line->next)
    {
    if( line->number==line_no &&
        line->version==version)
        return (CSTR_line)line;
     }

return (CSTR_line)0;
}

CSTR_FUNC(CSTR_line )   CSTR_GetLineFirst ( Int32 fragment_no, Int32 version)
{
CSTR_head *line;

for(line = head.next; line!=&tail; line=line->next)
    {
    if( line->attr.fragment==fragment_no &&
        line->version==version)
        return (CSTR_line)line;
     }

return (CSTR_line)0;
}

CSTR_FUNC(CSTR_line )   CSTR_GetLineNext (CSTR_line lin, Int32 fragment_no, Int32 version)
{
CSTR_head *line;

if( lin==(CSTR_line)0)
    {
    wLowRC=CSTR_ERR_NULL       ;
    return (CSTR_line)0;
    }
line = (CSTR_head *)lin;
for(line=line->next; line!=&tail; line=line->next)
    {
    if( line->attr.fragment==fragment_no &&
        line->version==version)
        return (CSTR_line)line;
     }

return (CSTR_line)0;
}

CSTR_FUNC(Int32)    CSTR_GetMaxNumber(void)
{
return num_lines;
}
/////////////////////////////////
// alloc/free & access to cells
/////////////////////////////////
CSTR_FUNC(CSTR_rast)    CSTR_GetFirstRaster (CSTR_line  linel)
{
CSTR_head *line = (CSTR_head *)linel;
if( line==(CSTR_head *)0 )
    {
    wLowRC=CSTR_ERR_NONEXIST ;
    return (CSTR_rast)NULL;
    }
return (CSTR_rast)(&line->first);
}


CSTR_FUNC(CSTR_rast)    CSTR_GetLastRaster (CSTR_line   linel)
{
CSTR_head *line = (CSTR_head *)linel;
if( line==(CSTR_head *)0 )
    {
    wLowRC=CSTR_ERR_NONEXIST ;
    return (CSTR_rast)NULL;
    }
return (CSTR_rast)(&line->last);
}


CSTR_FUNC(CSTR_rast)    CSTR_GetNextRaster (CSTR_rast  curr_raster, Word32 type_raster)
{
CSTR_cell *cell=(CSTR_cell *)curr_raster;
if( cell==(CSTR_cell *)0 )
    {
    wLowRC=CSTR_ERR_NONEXIST ;
    return (CSTR_rast)NULL;
    }
for(cell=cell->next; cell && !(cell->attr.flg&(CSTR_f_fict|type_raster)); cell=cell->next);
if( !cell )
    return 0;
return ( cell->attr.flg&type_raster ) ?
    (CSTR_rast)cell : (CSTR_rast)0;
}

CSTR_FUNC(CSTR_rast)    CSTR_GetPrevRaster (CSTR_rast  curr_raster, Word32 type_raster)
{
CSTR_cell *cell=(CSTR_cell *)curr_raster;
if( cell==(CSTR_cell *)0 )
    {
    wLowRC=CSTR_ERR_NONEXIST ;
    return (CSTR_rast)NULL;
    }
for(cell=cell->prev; cell && !(cell->attr.flg&(CSTR_f_fict|type_raster)); cell=cell->prev);
if( !cell )
    return 0;
return ( cell->attr.flg&type_raster ) ?
    (CSTR_rast)cell : (CSTR_rast)0;
}

CSTR_FUNC(CSTR_rast)    CSTR_NewRaster (CSTR_line linel, Int32 col, Int32 row, Int32 w)
{
CSTR_cell *cell, *start, *stop;
CSTR_head *line = (CSTR_head *)linel;
if( line==(CSTR_head *)0 )
    {
    wLowRC=CSTR_ERR_NONEXIST;
    return (CSTR_rast)NULL;
    }

start = &line->first;
stop  = &line->last;
for(start=start->next; start && start!=stop;start=start->next)
    {
    if( start->attr.col>=col )
        break;
    }
if( !start )
    {
    wLowRC=CSTR_ERR_NULL;
    return (CSTR_rast)NULL;
    }
if( start==stop || start->attr.col>col )
    {
    start = start->prev;
    }
else
    {// start->attr.col==col
    for(;start!=stop && start->attr.col==col;
        start=start->next)
        {
        if( start->attr.col+start->attr.w>=col+w )
            break;
        }
    for(;start!=stop && start->attr.col==col && start->attr.col+start->attr.col==col+w;
        start=start->next)
        {
        if( start->attr.row>=row )
            break;
        }
    start = start->prev;
    }
stop = start->next;
cell=my_alloc(sizeof(CSTR_cell));
if( !cell )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_rast)NULL;
    }
cell->attr.col = (Int16)col;
cell->attr.row = (Int16)row;
cell->attr.version = CSTR_VERSION_CODE;
cell->attr.n_baton= CSTR_NO_BATONS;
cell->attr.save_stick_inc= NO_INCLINE;
cell->attr.stick_inc= NO_INCLINE;
cell ->prev    = start;
cell ->next    = stop;
start->next    = cell;
stop ->prev    = cell;
cell->line_no  = linel;
cell->vers     = my_alloc(sizeof(UniVersions));
if( !cell->vers )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_rast)NULL;
    }

return (CSTR_rast)cell;
}

CSTR_FUNC(CSTR_rast)    CSTR_DelRaster (CSTR_rast  curr_raster)
{
CSTR_cell *cell  ;
CCOM_comp *env   ;
CSTR_cell *prev  ;
CSTR_cell *next  ;

if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return (CSTR_rast)0;
    }
cell=(CSTR_cell *)curr_raster;
if( cell->attr.col==32767 && (cell->attr.flg&CSTR_f_fict) )
    { //  fictive elm
    return (CSTR_rast)0;
    }
if( cell->attr.col==-16000 && (cell->attr.flg&CSTR_f_fict) )
    { //  fictive elm
    return (CSTR_rast)cell->next;
    }
//if( cell->next_down && cell->prev_down )
//        cstr_delete_branch(cell->next_down);
env  = cell->env ;
prev = cell->prev;
next = cell->next;
if( prev )
    prev->next = next;
if( next )
    next->prev = prev;
// delete branch raster
if( cell->next_up )
    {
    if( prev )
        {
        prev->next_up=cell->next_up;
        cell->next_up->prev_down=prev; // nick
        }
    else
        {
        cell->next_up->prev_down=0;
        cell->next_up=0;
        }
    }
if( cell->prev_up )
    {
    if( next )
        {
        next->prev_up=cell->prev_up;
        cell->prev_up->next_down=next; // nick
        }
    else
        {
        cell->prev_up->next_down=0;
        cell->prev_up=0;
        }
    }
// sync branch raster
if( cell->next_down )
        {
        cell->next_down->prev_up=next; // nick
        if( next )
                next->next_down = cell->next_down; // nick
        }
if( cell->prev_down )
        {
        cell->prev_down->next_up=prev; // nick
        if( prev )
                prev->prev_down = cell->prev_down; // nick
        }
if( cell->vers )
    my_free(cell->vers,0);
if( cell->recRaster )
    my_free(cell->recRaster,0);
if( env )
    CCOM_Delete( ((CSTR_head *)cell->line_no)->container, env);
my_free(cell,0);

return (CSTR_rast) next;
}

CSTR_FUNC(CSTR_line)    CSTR_GetLine (CSTR_rast  curr_raster)
{
return 0;
/*
CSTR_cell *cell  ;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return (CSTR_line)0;
    }
cell=(CSTR_cell *)curr_raster;
return cell->line_down;
*/
}

CSTR_FUNC(CSTR_line)    CSTR_GetRasterLine (CSTR_rast  curr_raster)
{
CSTR_cell *cell  ;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return (CSTR_line)0;
    }
cell=(CSTR_cell *)curr_raster;
return cell->line_no;
}

CSTR_FUNC(Bool32)       CSTR_StoreLine (CSTR_rast  curr_raster, CSTR_line linel)
{
/*
CSTR_cell *cell  ;
CSTR_head *line = (CSTR_head *)linel;

if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
if( line==(CSTR_head *)0 )
    {
    wLowRC=CSTR_ERR_NONEXIST;
    return FALSE;
    }
cell=(CSTR_cell *)curr_raster;
cell->line_down = linel;
return TRUE;
*/
return FALSE;
}

CSTR_FUNC(Bool32)           CSTR_GetAttr (CSTR_rast  curr_raster, CSTR_rast_attr * attr)
{
CSTR_cell *cell  ;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
cell=(CSTR_cell *)curr_raster;
*attr = cell->attr;
return TRUE;
}

CSTR_FUNC(Bool32)           CSTR_SetAttr (CSTR_rast  curr_raster, CSTR_rast_attr * attr)
{
CSTR_cell *cell  ;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
cell=(CSTR_cell *)curr_raster;
cell->attr = *attr ;
cell->attr.version = CSTR_VERSION_CODE;
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_SetUserAttr (CSTR_rast raster, CCOM_USER_BLOCK  *ubl)
{
CCOM_USER_BLOCK  *ub;
Word32      UserCode = ubl->code;
Word8   *   UserData = ubl->data;
Int32       UserSize = ubl->size;
CSTR_cell *cell  ;
if( raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
if( !UserSize || !UserData )
  {
  wLowRC=CSTR_ERR_NULL       ;
  return FALSE;
  }

cell=(CSTR_cell *)raster;
ub = cell->user_block;
if( ub )
while( ub )
  {
  if( ub->code==UserCode )
    {
    if( ub->data && ub->size )
      my_free(ub->data, ub->size);
    ub->data = (Word8*)my_alloc(UserSize);
    if( !ub->data )
      return FALSE;
    ub->size = UserSize;
    memcpy( ub->data, UserData, UserSize);
    return TRUE;
    }
  ub = ub->next_block;
  }
// ub==NULL
ub = (CCOM_USER_BLOCK*)my_alloc(sizeof(CCOM_USER_BLOCK));
if( !ub )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return FALSE;
    }
ub->data = (Word8*)my_alloc(UserSize);
if( !ub->data )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return FALSE;
    }
ub->size = UserSize;
memcpy( ub->data, UserData, UserSize);
return TRUE;
}


CSTR_FUNC(Bool32)               CSTR_GetUserAttr (CSTR_rast raster, CCOM_USER_BLOCK  *ubl)
{
CCOM_USER_BLOCK * ub;
CSTR_cell *cell  ;
if( raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
if( !ubl || !ubl->data )
    {
    wLowRC=CSTR_ERR_NULL;
    return FALSE;
    }
cell=(CSTR_cell *)raster;
if( ubl->code )
  {
  ub = cell->user_block;
  if( ub )
    while( ub )
      {
      if( ub->code==ubl->code )
        {
        ubl->data;
        ubl->size = ub->size ;
        return TRUE;
        }
      ub = ub->next_block;
      }
  }
wLowRC=CSTR_ERR_NONEXIST;
return FALSE;
}

Bool32 RecRaster2rst(RecRaster *recr,CSTR_cell  *cell)
{
int     n,n8,w,h;
Word8  *in, *out, *nin;

w=recr->lnPixWidth;
h=recr->lnPixHeight;
n=(w+7)/8;
n8=REC_GW_WORD8(w);
if( cell->recRaster )
    {
    my_free(cell->recRaster,0);
    cell->recRaster=0;
    }
cell->recRaster=my_alloc(h*n);
if( !cell->recRaster )
    return FALSE;
out =&cell->recRaster[0];
in  =&recr->Raster[0];
nin =in+n8*h;
for(;in<nin;out+=n,in+=n8)
    {
    memcpy(out,in,n);
    }
cell->lnPixWidth =w;
cell->lnPixHeight=h;
return TRUE;
}
// aligning from 1 byte to 8 bytes
Bool32 rst2RecRaster(CSTR_cell  *cell, RecRaster *recr)
{
int     n,n8,w,h;
Word8  *in, *out, *nin;
Word8   buf[256]={0};

w=cell->lnPixWidth;
h=cell->lnPixHeight;
n=(w+7)/8;
n8=REC_GW_WORD8(w);
in  =&cell->recRaster[0];
out =&recr->Raster[0];
nin =in+n*h;
for(;in<nin;in+=n,out+=n8)
    {
    memcpy(buf,in,n);
    memcpy(out,buf,n8);
    }
recr->lnPixWidth =w;
recr->lnPixHeight=h;
recr->lnRasterBufSize=REC_MAX_RASTER_SIZE;
return TRUE;
}

///////////////////////////////
// get / store representations
///////////////////////////////
CSTR_FUNC(Bool32)               CSTR_StoreRaster (CSTR_rast curr_raster, RecRaster *recr)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
CSTR_head   * line;
Word8         lp[6000];
Int16         lp_size;
Int16         multy;

if( curr_raster==(CSTR_rast)0 || recr==NULL )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

RecRaster2rst(recr,cell);
line = (CSTR_head*)(cell->line_no);
if( !(cell->env=CCOM_New(line->container, cell->attr.row, cell->attr.col, cell->attr.w, cell->attr.h)) )
    {
    wLowRC=CSTR_ERR_INTERNAL;
    return FALSE;
    }
if( !CCOM_MakeLP( recr , lp, &lp_size, &multy) )
    {
    wLowRC=CSTR_ERR_INTERNAL;
    return FALSE;
    }
CCOM_Store(cell->env,multy,lp_size, lp,  0, 0, 0, NULL,NULL);
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_StoreComp (CSTR_rast  curr_raster, Word8 *lp, Bool32 raster_init, Word8 scale)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
CCOM_comp   * comp;
CSTR_head   * line;
RecRaster     rs;
Int16        len, numc,num_ln;
Int16       *llen;
Word8       *l=lp;
CCOM_lnhead *ln;

if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
line = (CSTR_head*)(cell->line_no);
if( !(comp=CCOM_New(line->container, cell->attr.row, cell->attr.col, cell->attr.w, cell->attr.h)) )
    {
    wLowRC=CSTR_ERR_INTERNAL;
    return FALSE;
    }

for(num_ln=numc=len=0;;)
    {
    llen = (Int16*)l;
    len += *llen;
    if( *llen==0 )
        break;
    numc++;
    ln = (CCOM_lnhead *)(l+2);
    while( ln->lth )
        {
        num_ln++;
        ln = (CCOM_lnhead *)((char*)ln+ln->lth);
        }
    l+=*llen;
    }

CCOM_Store(comp,numc,len, lp, num_ln, 0, 0,  NULL,  NULL);
comp->scale = scale;
cell->env = comp;
if( raster_init )
    memset(&rs,0,sizeof(RecRaster));
rs.lnPixWidth   = comp->w;
rs.lnPixHeight  = comp->h;
if( comp->scale )
    {
    rs.lnPixWidth  =(rs.lnPixWidth +(1<<comp->scale)-1)>> comp->scale;
    rs.lnPixHeight =(rs.lnPixHeight+(1<<comp->scale)-1)>> comp->scale;
    }
if( REC_GW_WORD8(rs.lnPixWidth)*rs.lnPixHeight <REC_MAX_RASTER_SIZE )
    {
    if( !CCOM_AddLPToRaster  (comp, &rs) )
        {
        wLowRC=CSTR_ERR_TORASTER;
        return FALSE;
        }
    }
else
    {
    // can be warning about large comp
    rs.lnPixWidth  = 0;
    rs.lnPixHeight = 0;
    }
RecRaster2rst(&rs,cell);
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_StoreCompOriginal (CSTR_rast  curr_raster, CCOM_comp   * comp, Bool32 raster_init)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
RecRaster     rs;
Word8        scale=comp->scale;

if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

comp->scale = scale;
cell->env = comp;
if( raster_init )
    memset(&rs,0,sizeof(RecRaster));
rs.lnPixWidth   = comp->w;
rs.lnPixHeight  = comp->h;
if( comp->scale )
    {
    rs.lnPixWidth  =(rs.lnPixWidth +(1<<comp->scale)-1)>> comp->scale;
    rs.lnPixHeight =(rs.lnPixHeight+(1<<comp->scale)-1)>> comp->scale;
    }
if( REC_GW_WORD8(rs.lnPixWidth)*rs.lnPixHeight <REC_MAX_RASTER_SIZE )
    {
    if( !CCOM_AddLPToRaster  (comp, &rs) )
        {
        wLowRC=CSTR_ERR_TORASTER;
        return FALSE;
        }
    }
else
    {
    // can be warning about large comp
    rs.lnPixWidth  = 0;
    rs.lnPixHeight = 0;
    }
RecRaster2rst(&rs,cell);
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_StoreScale(CSTR_rast  curr_raster,Word8 scale)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
CCOM_comp   * comp;

if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
comp = cell->env;
if( comp==(CCOM_comp *)NULL )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
comp->scale = scale;
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_GetImage (CSTR_rast  curr_raster, Word8 *out_res, Word32 type_image)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
switch( type_image )
    {
    case    CSTR_TYPE_IMAGE_LP: // linepepresentation
        if( cell->env==NULL || cell->env->size_linerep<=0 )
            {
            wLowRC=CSTR_ERR_NULL       ;
            return FALSE;
            }
        memcpy(out_res, cell->env->linerep,cell->env->size_linerep);
        break;
    case    CSTR_TYPE_IMAGE_RS: // RecRaster
        if( cell->recRaster && ( cell->lnPixWidth==0 || cell->lnPixHeight==0) )
            {
            wLowRC=CSTR_ERR_NULL       ;
            return FALSE;
            }
        if( cell->env &&
            !cell->recRaster && cell->lnPixWidth && cell->lnPixHeight )
            {
            if( !cstr_CCOM2raster(cell->env, cell) )
                return FALSE;
            }
        rst2RecRaster(cell,(RecRaster*)out_res);
        ((RecRaster*)out_res)->lnRasterBufSize = REC_MAX_RASTER_SIZE;
        break;
    case    CSTR_TYPE_IMAGE_RS1: // B/W bitmap aligned to 1 byte
        if( cell->recRaster && ( cell->lnPixWidth==0 || cell->lnPixHeight==0) )
            {
            wLowRC=CSTR_ERR_NULL       ;
            return FALSE;
            }
        if( cell->env &&
            !cell->recRaster && cell->lnPixWidth && cell->lnPixHeight )
            {
            if( !cstr_CCOM2raster(cell->env, cell) )
                return FALSE;
            }
        memcpy(out_res,cell->recRaster,((cell->lnPixWidth+7)/8)*cell->lnPixHeight);
        break;
    default:
        wLowRC=CSTR_ERR_VALUE       ;
        return FALSE;
    }
return TRUE;
}

CSTR_FUNC(Bool32)               CSTR_VerifyLine (CSTR_line      line)
{
CSTR_rast       start    = CSTR_GetFirstRaster (line), stop = CSTR_GetLastRaster (line);
CSTR_cell      *f, *l, *c;
int             n,i;
Bool32          retl, ret;
CSTR_rast       rn;
RecRaster       rs;
RecVersions     vs;

if( !start || !stop )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
for(n=0,f=(CSTR_cell*)start,l=(CSTR_cell*)stop,c=f->next;c!=l;c=c->next)
    {
    if( c->attr.flg==(CSTR_f_bad|CSTR_f_dust) )
        c->attr.flg=CSTR_f_bad;
    n++;
    }
ret=TRUE;
if( n>1 )
{
l = l->prev;
for(c=f->next;c!=l;c=c->next)
    {
    if( c->next->attr.col<c->attr.col )
        {
        retl =TRUE;
        rn=CSTR_NewRaster(line,c->next->attr.col, c->next->attr.row, c->next->attr.w);
        if( rn=(CSTR_rast)0 )
            retl=FALSE;

        if( !CSTR_SetAttr(rn, &c->attr) )
            retl=FALSE;
        vs.lnAltCnt=c->vers->lnAltCnt;
        for(i=0;i<c->vers->lnAltCnt;i++)
            {
            vs.Alt[i].Code      =c->vers->Alt[i].Liga;
            vs.Alt[i].CodeExt   =0;
            vs.Alt[i].Method    =c->vers->Alt[i].Method;
            vs.Alt[i].Prob      =c->vers->Alt[i].Prob;
            vs.Alt[i].Info      =c->vers->Alt[i].Info;
            }
        if( !CSTR_StoreCollection(rn, &vs) )
            retl=FALSE;
        rst2RecRaster(c,&rs);
        if( !CSTR_StoreRaster(rn, &rs) )
            retl=FALSE;
        if( retl )
            CSTR_DelRaster((CSTR_rast)c);
        else
            ret=FALSE;
        }
    }
}

if( !ret )
    {
    wLowRC=CSTR_ERR_FAILURE;
    return FALSE;
    }

return ret;
}

CSTR_FUNC(Bool32)                   CSTR_StoreCollection (CSTR_rast  curr_raster, RecVersions *collect)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
UniVersions uvs;
int i;

if( !collect )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return TRUE;
    }
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    uvs.lnAltCnt=0;
    return FALSE;
    }
uvs.lnAltCnt=collect->lnAltCnt;
for(i=0;i<collect->lnAltCnt;i++)
    {
    uvs.Alt[i].Code[0]   =collect->Alt[i].Code;
    uvs.Alt[i].Code[1]   ='\0';
    uvs.Alt[i].Liga      =collect->Alt[i].Code;
    uvs.Alt[i].Charset   =0;
    uvs.Alt[i].Method    =collect->Alt[i].Method;
    uvs.Alt[i].Prob      =collect->Alt[i].Prob;
    uvs.Alt[i].Info      =collect->Alt[i].Info;
    }
if( !cell->vers )
    {
    cell->vers=my_alloc(sizeof(UniVersions));
    if( !cell->vers )
        {
        wLowRC=CSTR_ERR_NOMEMORY ;
        return FALSE;
        }
    }
uvs.lnAltMax=REC_MAX_VERS;
memcpy(cell->vers,&uvs,sizeof(UniVersions));

cell->attr.flg_new|=CSTR_fn_initvers;
return TRUE;
}

CSTR_FUNC(Bool32)                   CSTR_StoreCollectionUni (CSTR_rast  curr_raster, UniVersions *collect)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;

if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
if( !cell->vers )
    {
    cell->vers=my_alloc(sizeof(UniVersions));
    if( !cell->vers )
        {
        wLowRC=CSTR_ERR_NOMEMORY ;
        return FALSE;
        }
    }
collect->lnAltMax=REC_MAX_VERS;
memcpy(cell->vers,collect,sizeof(UniVersions));

cell->attr.flg_new|=CSTR_fn_initvers;
return TRUE;
}

CSTR_FUNC(Bool32)                   CSTR_GetCollection (CSTR_rast  curr_raster, RecVersions *result_collect)
{
int i;
RecVersions vs;
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

if( !cell->vers )
    {
    vs.lnAltCnt=0;
    }
else

    {
    vs.lnAltCnt=cell->vers->lnAltCnt;
    for(i=0;i<cell->vers->lnAltCnt;i++)
        {
        vs.Alt[i].Code      =cell->vers->Alt[i].Liga;
        vs.Alt[i].CodeExt   =0;
        vs.Alt[i].Method    =cell->vers->Alt[i].Method;
        vs.Alt[i].Prob      =cell->vers->Alt[i].Prob;
        vs.Alt[i].Info      =cell->vers->Alt[i].Info;
        }
    }
if( !vs.lnAltCnt )
    {
    if( (cell->attr.flg_new&CSTR_fn_initvers)
    && cell->vers    )
        {
        vs.Alt[0].Code      =   cell->vers->Alt[0].Liga;
        vs.Alt[0].CodeExt   =0;
        vs.Alt[0].Method    =cell->vers->Alt[0].Method;
        vs.Alt[0].Prob      =cell->vers->Alt[0].Prob;
        vs.Alt[0].Info      =cell->vers->Alt[0].Info;
        }
    else
        {
        vs.Alt[0].Code      =   '~';
        vs.Alt[0].CodeExt   =0;
        vs.Alt[0].Method    =0;
        vs.Alt[0].Prob      =0;
        vs.Alt[0].Info      =0;
        }
    }
memcpy(result_collect,&vs,sizeof(RecVersions));
result_collect->lnAltMax=REC_MAX_VERS;
return TRUE;
}

CSTR_FUNC(Bool32)                   CSTR_GetCollectionUni (CSTR_rast  curr_raster, UniVersions *result_collect)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

if( !cell->vers )
    {
    result_collect->lnAltCnt = 0;
    result_collect->Alt[0].Code[0]   =   '~';
    result_collect->Alt[0].Code[1]   =0;
    result_collect->Alt[0].Liga      =   '~';
    result_collect->Alt[0].Method    =0;
    result_collect->Alt[0].Prob      =0;
    result_collect->Alt[0].Info      =0;
    }
else
    memcpy(result_collect,cell->vers,sizeof(UniVersions));
result_collect->lnAltMax=REC_MAX_VERS;
return TRUE;
}

CSTR_FUNC(CCOM_comp *)      CSTR_GetComp (CSTR_rast  curr_raster)
{
CSTR_cell   * cell = (CSTR_cell*)curr_raster;
if( curr_raster==(CSTR_rast)0 )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
return cell->env;
}

CSTR_FUNC(Int32)     CSTR_NewUserCode (void)
{
user_number++;
return user_number;
}

CSTR_FUNC(Bool32)                   CSTR_LineToTxt (CSTR_line   lin, char *txt)
{
CSTR_rast       start = CSTR_GetFirstRaster (lin),
                stop = CSTR_GetLastRaster (lin), c;
UniVersions     vers;
CSTR_rast_attr  attr;

if( !start || !stop )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
c=CSTR_GetNextRaster (start,CSTR_f_all);
for(*txt='\0'; c && c!=stop; c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    CSTR_GetAttr(c, &attr);
    if( !(attr.flg & (CSTR_f_let|CSTR_f_punct|CSTR_f_bad|CSTR_f_space|CSTR_f_solid)) )
        continue;
    if( CSTR_GetCollectionUni(c, &vers) )
        {
        if( !vers.lnAltCnt )
                    strcat(txt,"~");
        else
                    strcat(txt,vers.Alt[0].Code);
        }
    }

return TRUE;
}

CSTR_FUNC(Bool32)                   CSTR_LineToTxt_Coord (CSTR_line   lin, char *txt, Int32 len)
{
CSTR_rast       start = CSTR_GetFirstRaster (lin),
                stop = CSTR_GetLastRaster (lin), c;
UniVersions     vers;
CSTR_rast_attr  attr;
CSTR_attr       lattr;
char            buf[20];

if( !start || !stop )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
c=CSTR_GetNextRaster (start,CSTR_f_all);
CSTR_GetLineAttr(lin,&lattr);
sprintf(txt,"#%d %d %d %d#",lattr.r_col,lattr.r_row,lattr.r_col+lattr.r_wid,lattr.r_row+lattr.r_hei);
for(; c && c!=stop; c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    CSTR_GetAttr(c, &attr);
    if( !(attr.flg & (CSTR_f_let|CSTR_f_punct|CSTR_f_bad|CSTR_f_space|CSTR_f_solid)) )
        continue;
    if( CSTR_GetCollectionUni(c, &vers) )
        {
        if( !vers.lnAltCnt )
            strcpy(buf,"~");
        else
            strcpy(buf,vers.Alt[0].Code);
        if( (Int32)(strlen(txt)+strlen(buf))<len )
            strcat(txt,buf);
        else
            return FALSE;
        }
    }

return TRUE;
}

CSTR_FUNC(Int32)                   CSTR_GetLength (CSTR_line   lin)
{
CSTR_rast       c;
Int32           len;
UniVersions     vers;
CSTR_rast_attr  attr;

if( !lin )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
c=CSTR_GetNext( CSTR_GetFirstRaster (lin) );
for(len=0; c ; c=CSTR_GetNext(c))
{
    CSTR_GetAttr(c, &attr);
    if( !(attr.flg & (CSTR_f_let|CSTR_f_punct|CSTR_f_bad|CSTR_f_space|CSTR_f_solid)) )
        continue;
    if( CSTR_GetCollectionUni(c, &vers) )
        {
        if( !vers.lnAltCnt )
            len++;
        else
            len+=strlen(vers.Alt[0].Code);
    }
}

return len;
}

CSTR_FUNC(CSTR_rast)            CSTR_InsertRaster (CSTR_rast    curr_raster)
{
CSTR_cell   * start = (CSTR_cell*)curr_raster, *stop, *stopn, *cell;

if( !start )
    {
    wLowRC=CSTR_ERR_NONEXIST;
    return (CSTR_rast)NULL;
    }
stop = start->next;
stopn= start->next_up ;
if( !stop && !stopn )
    {
    wLowRC=CSTR_ERR_NONEXIST;
    return (CSTR_rast)NULL;
    }

cell=my_alloc(sizeof(CSTR_cell));
if( !cell )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_rast)NULL;
    }

cell ->prev    = start;
start->next    = cell;
if( stop )
        {
        cell ->next    = stop;
        stop ->prev    = cell;
        }
if( stopn )
    {
    cell->next_up=stopn;
    stopn->prev_down=cell;
    start->next_up=0;
    }
cell->line_no  = start->line_no;
cell->vers     = my_alloc(sizeof(UniVersions));
if( !cell->vers )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_rast)NULL;
    }

return (CSTR_rast)cell;
}

CSTR_FUNC(CSTR_rast)            CSTR_InsertRasterDown (CSTR_rast        start_raster, CSTR_rast stop_raster)
{
CSTR_cell   *   start = (CSTR_cell*)start_raster,*   stop = (CSTR_cell*)stop_raster,  *cell;

if( !start  || !stop)
    {
    wLowRC=CSTR_ERR_NONEXIST;
    return (CSTR_rast)NULL;
    }

cell=my_alloc(sizeof(CSTR_cell));
if( !cell )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_rast)NULL;
    }

cell ->prev_up    = start;
cell ->next_up    = stop;
start->next_down  = cell;
stop ->prev_down  = cell;
cell->line_no  = start->line_no;
cell->vers     = my_alloc(sizeof(UniVersions));
if( !cell->vers )
    {
    wLowRC=CSTR_ERR_NOMEMORY        ;
    return (CSTR_rast)NULL;
    }
cell->attr.flg_new |= CSTR_fn_down ;
return (CSTR_rast)cell;
}

CSTR_FUNC(CSTR_rast)    CSTR_GetNext(CSTR_rast rst)
{
return CSTR_GetNextRaster(rst,CSTR_f_all);
}

CSTR_FUNC(CSTR_rast)    CSTR_GetNextDown(CSTR_rast rst)
{
return ((CSTR_cell*)rst)->next_down;
}

CSTR_FUNC(CSTR_rast)    CSTR_GetPrevDown(CSTR_rast rst)
{
return ((CSTR_cell*)rst)->prev_down;
}

CSTR_FUNC(CSTR_rast)    CSTR_GetNextUp(CSTR_rast rst)
{
return ((CSTR_cell*)rst)->next_up;
}

CSTR_FUNC(CSTR_rast)    CSTR_GetPrevUp(CSTR_rast rst)
{
return ((CSTR_cell*)rst)->prev_up;
}

CSTR_FUNC(CSTR_rast)    CSTR_GetPrev(CSTR_rast rst)
{
return CSTR_GetPrevRaster(rst,CSTR_f_all);
}

CSTR_FUNC(void) CSTR_DeleteAll(void)
{
CSTR_head       *line=head.next, *nline;
// delete all heads
for( ;line && line!=&tail;  )
    {
    nline   =   line->next;
    CSTR_DeleteLine ((CSTR_line)line);
    line    =   nline;
    }

wLowRC = CSTR_ERR_NO;
num_lines = 0;
user_number = 0xFFFFFF+1;
memset(FragmFirst0,0,8000*sizeof(CSTR_line));
memset(FragmFirst1,0,8000*sizeof(CSTR_line));
FragmMin[0]=FragmMin[1]=16000;
FragmMax[0]=FragmMax[1]=-1;
return;
}

CSTR_FUNC(Bool32) CSTR_SwapRasters(CSTR_rast r1, CSTR_rast r2)
{
CSTR_cell *c1=(CSTR_cell *)r1, *c2=(CSTR_cell *)r2, *c1p, *c1n, *c2p, *c2n;
if( c1->attr.flg==CSTR_f_fict || c2->attr.flg==CSTR_f_fict )
    return FALSE;
c1p = c1->prev;
c1n = c1->next;
c2p = c2->prev;
c2n = c2->next;

c1p->next = c2;
c1n->prev = c2;
c2p->next = c1;
c2n->prev = c1;

c1->next = c2n;
c1->prev = c2p;
c2->next = c1n;
c2->prev = c1p;

return TRUE;
}

CSTR_FUNC(CSTR_rast)    CSTR_SaveRaster(CSTR_rast c)
{
CSTR_rast   rn;
RecRaster   rs;
RecVersions vr;
CSTR_rast_attr attr;
CSTR_line   ln;
if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return 0;
    }
ln = CSTR_GetLineHandle(0xFFFFFFFF, 0xFFFFFFFF);
if( !ln )
    {
    if( !CSTR_NewLine (0xFFFFFFFF, 0xFFFFFFFF, -1) )
        return 0;
    ln = CSTR_GetLineHandle(0xFFFFFFFF, 0xFFFFFFFF);
    if( !ln )
        {
        wLowRC=CSTR_ERR_NONEXIST;
        return 0;
        }
    }

if( CSTR_GetAttr  (c, &attr) &&
    CSTR_GetImage (c, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) &&
    CSTR_GetCollection(c,&vr) )
    {
    if( !(rn=CSTR_NewRaster (ln ,attr.col, attr.row,attr.w)) )
        return 0;
    if( !CSTR_SetAttr (rn, &attr) )
        return 0;
    if( !CSTR_StoreRaster (rn, &rs) )
        return 0;
    if( !CSTR_StoreCollection (rn, &vr) )
        return 0;
    }
return rn;
}

CSTR_FUNC(CSTR_rast)    CSTR_DelSaveRaster(CSTR_rast c)
{
CSTR_rast   rn=CSTR_SaveRaster(c);
CSTR_DelRaster(c);
return rn;
}

CSTR_FUNC(CSTR_rast) CSTR_RestoreRaster(CSTR_line ln,CSTR_rast rst)
{
CSTR_rast   rn;
RecRaster   rs;
RecVersions vr;
CSTR_rast_attr attr;

if( !rst || !ln )
    {
    wLowRC=CSTR_ERR_NULL;
    return 0;
    }

if( CSTR_GetAttr  (rst, &attr) &&
    CSTR_GetImage (rst, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) &&
    CSTR_GetCollection(rst,&vr) )
    {
    if( !(rn=CSTR_NewRaster (ln ,attr.col, attr.row,attr.w)) )
        return 0;
    if( !CSTR_SetAttr (rn, &attr) )
        return 0;
    if( !CSTR_StoreRaster (rn, &rs) )
        return 0;
    if( !CSTR_StoreCollection (rn, &vr) )
        return 0;
    }
CSTR_DelRaster(rst);
return rn;
}

CSTR_FUNC(CSTR_rast) CSTR_GetComplist(CSTR_rast rst)
{
CSTR_cell * c=(CSTR_cell *)rst;
if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return 0;
    }

return (CSTR_rast)c->complist;
}

CSTR_FUNC(Bool32) CSTR_SetComplist(CSTR_rast rst, CSTR_rast complist)
{
CSTR_cell * c=(CSTR_cell *)rst;
if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return FALSE;
    }
c->complist = (CSTR_cell*)complist;
return TRUE;
}

CSTR_FUNC(CSTR_rast) CSTR_compose_Cell(Int32 n,CSTR_rast *clist, Int32 nIncline, Bool32 NeedDel)
{
CSTR_rast_attr attr;
Int16   minrow, mincol, maxcol, maxrow;
RecRaster   rst;
CCOM_comp   *comp;
CSTR_cell   *cell;
Int32      i;
CSTR_line line_no;
CSTR_rast   c;

if( !n || !clist )
    {
    wLowRC=CSTR_ERR_NULL;
    return 0;
    }
for(minrow=mincol=10000,maxrow=maxcol=0,i=0;i<n;i++)
    {
    if( !clist[i] )
        {
        wLowRC=CSTR_ERR_NULL;
        return 0;
        }
    CSTR_GetAttr(clist[i],&attr);
    if( attr.r_row<minrow )
        minrow = attr.r_row;
    if( attr.r_row+attr.h>maxrow )
        maxrow = attr.r_row+attr.h;
    if( attr.r_col<mincol )
        mincol = attr.r_col;
    if( attr.r_col+attr.w>maxcol )
        maxcol = attr.r_col+attr.w;
    cell = (CSTR_cell*)clist[i];
    if( !i )
        line_no = cell->line_no;
    else if( line_no!=cell->line_no )
        {
        wLowRC=CSTR_ERR_VALUE;
        return 0;
        }
    }
maxrow -= minrow;
maxcol -= mincol;

memset(&attr,0,sizeof(CSTR_rast_attr));
attr.r_row = minrow;
attr.r_col = mincol;
attr.h   = maxrow;
attr.w   = maxcol;
attr.row=attr.r_row-(Int16)(nIncline*attr.r_col/2048);
attr.col=attr.r_col+(Int16)(nIncline*attr.r_row/2048);
memset(&rst.Raster[0],0,REC_MAX_RASTER_SIZE );
rst.lnRasterBufSize=REC_MAX_RASTER_SIZE   ;
rst.lnPixWidth  = maxcol;
rst.lnPixHeight = maxrow;
for(i=0;i<n;i++)
    {
    comp = CSTR_GetComp(clist[i]);
    CCOM_AddLPToRaster(comp, &rst); // can be compress case
    if( NeedDel )
        CSTR_DelRaster(clist[i]);
    }
c = CSTR_NewRaster(line_no,attr.col, attr.row,attr.w);
if( !c )
    return 0;
CSTR_SetAttr(c,&attr);
if( !CSTR_StoreRaster (c, &rst) )
    return 0;
return c;
}

CSTR_FUNC(Bool32) CSTR_KillImage(CSTR_rast rst)
{
CSTR_cell *c=(CSTR_cell*)rst;
CCOM_Delete(((CSTR_head*)(c->line_no))->container,c->env);
c->lnPixHeight = c->lnPixWidth = 0;
my_free(c->recRaster,0);
c->recRaster=0;
return 0;
}

CSTR_FUNC(CCOM_handle) CSTR_GetContainer(CSTR_line ln)
{
CSTR_head       *line=(CSTR_head        *)ln;
return line->container;
}

CSTR_FUNC(Bool32) CSTR_EmptyLine(CSTR_line lin)
{
	CSTR_rast start = CSTR_GetFirstRaster (lin),stop = CSTR_GetLastRaster (lin), c;
	for(c=CSTR_GetNextRaster (start,CSTR_f_all);c&&c!=stop;c = CSTR_DelRaster(c));
	return TRUE;
}

CSTR_FUNC(Bool32) CSTR_ClearLine(CSTR_line lin,Int16 left, Int16 right)
{
CSTR_rast start = CSTR_GetFirstRaster (lin),stop = CSTR_GetLastRaster (lin), c;
CSTR_rast_attr  attr;

if( !start || !stop )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }
for(c=CSTR_GetNextRaster (start,CSTR_f_all);c&&c!=stop;c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    CSTR_GetAttr(c,&attr);
    if( attr.r_col>=left )
        break;
    }
for(; c && c!=stop; )
    {
    CSTR_GetAttr(c,&attr);
    if( attr.r_col+attr.w>= right )
        break;
    c = CSTR_DelRaster (c);
    }

return TRUE;
}


CSTR_FUNC(CSTR_rast) CSTR_GetDup(CSTR_rast rus)
{
CSTR_cell   *c=(CSTR_cell*)rus;

if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return (CSTR_rast)0;
    }
return c->dup;
}

CSTR_FUNC(CSTR_rast) CSTR_GetDupEnd(CSTR_rast rus)
{
CSTR_cell   *c=(CSTR_cell*)rus;

if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return (CSTR_rast)0;
    }
return c->dupend;
}

CSTR_FUNC(Bool32) CSTR_SetDup(CSTR_rast   rus, CSTR_rast   eng)
{
CSTR_cell   *c=(CSTR_cell*)rus;

if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return FALSE;
    }
c -> dup = eng;
return TRUE;
}

CSTR_FUNC(Bool32) CSTR_SetDupEnd(CSTR_rast   rus,CSTR_rast    eng)
{
CSTR_cell   *c=(CSTR_cell*)rus;

if( !c )
    {
    wLowRC=CSTR_ERR_NULL;
    return FALSE;
    }
c -> dupend = eng;
return TRUE;
}


Bool32 cstr_unpack_cell(CSTR_cell *cell)
{
if( cell->env &&
        !cell->recRaster && cell->lnPixWidth && cell->lnPixHeight )
    {
    if( !cstr_CCOM2raster(cell->env, cell) )
                return FALSE;
        }
if( !cell->vers )
    {
    cell->vers=my_alloc(sizeof(UniVersions));
    }
return TRUE;
}

Bool32 cstr_pack_cell(CSTR_cell *cell)
{
if( cell->env &&
    cell->recRaster && cell->lnPixWidth && cell->lnPixHeight )
    {
    if( !cstr_delete_raster(cell) )
        return FALSE;
    }

if( cell->vers && cell->vers->lnAltCnt<1 )
    {
    my_free(cell->vers,0);
    cell->vers=0;
    }
return TRUE;
}

Bool32 cstr_pack_unpack_branch(CSTR_cell *cell, Bool32 (*fun_pack_unpack)(CSTR_cell *c))
{
do      {
        if( !fun_pack_unpack(cell) )
        return FALSE;
        if( cell->next_up )
                break;
        cell=cell->next;
        }       while( cell );
return TRUE;
}

CSTR_FUNC(Bool32) CSTR_PackLine(CSTR_line line)
{
CSTR_rast   rast;
CSTR_cell  *cell;

if( line==(CSTR_line)0)
    {
    wLowRC=CSTR_ERR_NULL       ;
    return (CSTR_line)0;
    }

for(rast=CSTR_GetNext(CSTR_GetFirstRaster(line));rast;rast=CSTR_GetNext(rast))
    {
    cell=(CSTR_cell*)rast;
    if( !cstr_pack_cell(cell) )
        return FALSE;
    if( cell->next_down )
                {
                if( !cstr_pack_unpack_branch(cell->next_down, cstr_pack_cell) )
                        return FALSE;
                }
    }
return TRUE;
}

CSTR_FUNC(Bool32) CSTR_UnpackLine(CSTR_line line)
{
CSTR_rast   rast;
CSTR_cell  *cell;

if( line==(CSTR_line)0)
    {
    wLowRC=CSTR_ERR_NULL       ;
    return (CSTR_line)0;
    }

for(rast=CSTR_GetNext(CSTR_GetFirstRaster(line));rast;rast=CSTR_GetNext(rast))
    {
    cell=(CSTR_cell*)rast;
    if( !cstr_unpack_cell(cell) )
        return FALSE;
        if( cell->next_down )
                {
                if( cstr_pack_unpack_branch(cell->next_down, cstr_unpack_cell) )
                        return FALSE;
                }
       // }

    }
return TRUE;
}


CSTR_FUNC(Bool32)       CSTR_SortFragm( Int32 version)
{
CSTR_head       *line;

if( version!=1 && version!=0 )
    {
    wLowRC = CSTR_ERR_VALUE;
    return FALSE;
    }
memset(FragmFirst1,0,8000*sizeof(CSTR_line));
memset(FragmFirst0,0,8000*sizeof(CSTR_line));
FragmMin[0]=FragmMin[1]=16000;
FragmMax[0]=FragmMax[1]=-1;
num_fragments1=num_fragments0=0;
if( version )
    {

    for(line=head.next; line!=&tail ; line=line->next)
        {
        if( line->version==version )
            {
            if( FragmMin[version]>line->attr.fragment )
                FragmMin[version]=line->attr.fragment ;
            if( FragmMax[version]<line->attr.fragment )
                FragmMax[version]=line->attr.fragment ;
            if( num_fragments1<line->attr.fragment )
                num_fragments1      = line->attr.fragment;
            if( !FragmFirst1[line->attr.fragment] )
                {
                FragmFirst1[line->attr.fragment]=(CSTR_line)line;
                }
            else
                {
                ((CSTR_head*)FragmLast1 [line->attr.fragment])->next_fragm_line = line;
                }
            FragmLast1 [line->attr.fragment]=(CSTR_line)line;
            }
        }
    }
else
    {
    for(line=head.next; line!=&tail ; line=line->next)
        {
        if( line->version==version )
            {
            if( FragmMin[version]>line->attr.fragment )
                FragmMin[version]=line->attr.fragment ;
            if( FragmMax[version]<line->attr.fragment )
                FragmMax[version]=line->attr.fragment ;
            if( num_fragments0<line->attr.fragment )
                num_fragments0      = line->attr.fragment;
            if( !FragmFirst0[line->attr.fragment] )
                {
                FragmFirst0[line->attr.fragment]=(CSTR_line)line;
                }
            else
                {
                ((CSTR_head*)FragmLast0 [line->attr.fragment])->next_fragm_line = line;
                }
            FragmLast0 [line->attr.fragment]=(CSTR_line)line;
            }
        }
    }
return TRUE;
}

CSTR_FUNC(Int32)    CSTR_GetMaxFragment(Int32 version)
{
return                version ? num_fragments1 : num_fragments0   ;
}

CSTR_FUNC(CSTR_line)    CSTR_FirstLineFragm( Int32 fragm, Int32 version )
{
if( version!=1 && version!=0 )
    {
    wLowRC = CSTR_ERR_VALUE;
    return 0;
    }
if( version )
    return FragmFirst1[fragm];
return FragmFirst0[fragm];
}

CSTR_FUNC(CSTR_line)    CSTR_NextLineFragm( CSTR_line start)
{
CSTR_head       *line=(CSTR_head        *)start;
if( !start )
    {
    wLowRC = CSTR_ERR_NULL;
    return 0;
    }
return (CSTR_line)line->next_fragm_line;
}

CSTR_FUNC(Bool32)       CSTR_CopyRaster(CSTR_rast       trg,CSTR_rast   src)
{
CSTR_rast_attr  attr;
RecRaster       rs;
UniVersions     vr;
CCOM_comp    *  comp;

if( !trg || !src )
    {
    wLowRC=CSTR_ERR_NULL       ;
    return FALSE;
    }

if( !CSTR_GetAttr (src, &attr) )
        return FALSE;
if( !CSTR_SetAttr (trg, &attr) )
    return FALSE;
if(  CSTR_GetImage (src, (Word8 *)&rs, CSTR_TYPE_IMAGE_RS) &&
    (comp=CSTR_GetComp(src))!=NULL )
        {
        if( !CSTR_StoreRaster (trg, &rs) )
        return FALSE;
        if( !CSTR_StoreScale(trg,comp->scale) )
                return FALSE;
        }

if( CSTR_GetCollectionUni(src,&vr)  )
    {
    if( !CSTR_StoreCollectionUni (trg, &vr) )
        return FALSE;

    }

return TRUE;
}


CSTR_FUNC(Bool32) CSTR_GetExportData(Word32 dwType, void * pData)
{
        Bool32 rc = TRUE;
    Int32  vers = CSTR_VERSION_CODE;
#define EXPORT(name) *(Word32*)(pData)=(Word32)name;
        wLowRC = CSTR_ERR_NO;
        switch(dwType)
        {
    case    CSTR_FNNEWLINE: //      ���������� ����� ������
        EXPORT(CSTR_NewLine );
        break;
    case    CSTR_FNCLEARLINE: // ������� ��� ������ �� ������
        EXPORT(CSTR_ClearLine);
        break;
    case    CSTR_FNDELLINE: //      ������� ������
        EXPORT(CSTR_DeleteLine );
        break;
    case    CSTR_FNSETSTRATTR: //   ���������� �������� ������� ������
        EXPORT(CSTR_SetLineAttr);
        break;
    case    CSTR_FNGETSTRATTR: //   �������� �������� ������� ������
        EXPORT(CSTR_GetLineAttr);
        break;
    case    CSTR_FNGETFIRST: //     ���� ����� ������� (����������) ������ � ������
        EXPORT(CSTR_GetFirstRaster );
        break;
    case    CSTR_FNGETLAST: //      ���� ����� ���������� (����������) ������ � ������
        EXPORT(CSTR_GetLastRaster);
        break;
    case    CSTR_FNGETNEXT: //      ���� ����� ���������� ������
        EXPORT(CSTR_GetNextRaster);
        break;
    case    CSTR_FNGETPREV: //      ���� ����� ����������� ������
        EXPORT(CSTR_GetPrevRaster);
        break;
    case    CSTR_FNGETLINE: //      ���� ����� ������, ���������� � �������
        wLowRC = CSTR_ERR_NOTIMPLEMENT;
                rc = FALSE;
        break;
    case    CSTR_FNSTORELINE: //    ������� ������ ������
        wLowRC = CSTR_ERR_NOTIMPLEMENT;
                rc = FALSE;
        break;
    case    CSTR_FNNEWRASTER: //    ������� ����� ����� � ������� ������
        EXPORT(CSTR_NewRaster );
        break;
    case    CSTR_FNSTORERASTER: //  ��������� ����� ��� �����������
        EXPORT(CSTR_StoreRaster );
        break;
    case    CSTR_FNSTORECOMP: // ��������� ����� ��� ���������� � ������������ ��������������
        EXPORT(CSTR_StoreComp  );
        break;
    case    CSTR_FNSTORECOLLECTION: // ��������� ���������
        EXPORT(CSTR_StoreCollection );
        break;
    case    CSTR_FNDELRASTER: //    ������� �����
        EXPORT(CSTR_DelRaster );
        break;
    case    CSTR_FNVERIFY: //       ������������� ������ ������
        EXPORT(CSTR_VerifyLine );
        break;
    case    CSTR_FNGETRASTER: //    �������� ����������� (����� ��� ���������)
        EXPORT(CSTR_GetImage );
        break;
    case    CSTR_FNGETATTR: //      �������� �������� ������
        EXPORT(CSTR_GetAttr );
        break;
    case    CSTR_FNGETCOLLECT: //   �������� ���������
        EXPORT(CSTR_GetCollection );
        break;
    case    CSTR_FNSETUSER: //      �������� � ����� ���������������� ������
        EXPORT(CSTR_SetUserAttr );
        break;
    case    CSTR_FNGETUSER: //      ��������� �� ������ ���������������� ������
        EXPORT(CSTR_GetUserAttr );
        break;
    case    CSTR_FNSETATTR: //      �������� �������� ������
        EXPORT(CSTR_SetAttr );
        break;
    case    CSTR_FNCOPYLINE: //     ����������� ���������� ������
        EXPORT(CSTR_CopyLine );
        break;
    case    CSTR_FNGETLNHND: //     ���� ����� ������ �� ������ � ������
        EXPORT(CSTR_GetLineHandle );
        break;
    case    CSTR_FNNEWUSERCODE: //  ���� ��������� ��� ��� USER cod-a
        EXPORT(CSTR_NewUserCode);
        break;
    case    CSTR_FNVERSION:     //      ���� ������ ����������
        EXPORT(vers);
        break;
    case    CSTR_FNTOTXT:       //  �������� ��������� ����� ������
        EXPORT(CSTR_LineToTxt );
        break;
    case    CSTR_FNINSRST:      //  �������� ����� ����� ��������
        EXPORT(CSTR_InsertRaster  );
        break;
    case    CSTR_FNMAXNUM:      //  ���������� ����� ������ � ����������
        EXPORT(CSTR_GetMaxNumber );
        break;
    case    CSTR_FNFIRSTLN:     //   ������ ������ ��������� ������
        EXPORT(CSTR_FirstLine );
        break;
    case    CSTR_FNNEXTTLN:     //   ��������� ������ ��������� ������
        EXPORT(CSTR_NextLine );
        break;
    case    CSTR_FNDELALL:      //  ������� ��� ������
        EXPORT(CSTR_DeleteAll);
        break;
    case    CSTR_FNNEXT:        // ��������� ����������� �����
        EXPORT(CSTR_GetNext);
        break;
    case    CSTR_FNPREV:        // ���������� ����������� �����
        EXPORT(CSTR_GetPrev);
        break;
    case    CSTR_FNSWAP :       // �������� ������� 2 ������
        EXPORT(CSTR_SwapRasters);
        break;
    case    CSTR_FNDELSAVE:     // ������� � �����������
        EXPORT(CSTR_DelSaveRaster);
        break;
    case    CSTR_FNRESTRAST:    // ������������
        EXPORT(CSTR_RestoreRaster);
        break;
    case    CSTR_FNCOMPOSE  :   // ������� ������ �������
        EXPORT(CSTR_compose_Cell);
        break;
    case    CSTR_KILLIM :       // ����� ����� � ������
        EXPORT(CSTR_KillImage);
        break;
    case      CSTR_FNGETCOMP:  // ���� ����������
        EXPORT(CSTR_GetComp );
        break;
	case    CSTR_FNTOTXT_COORD: //  �������� ��������� ����� ������ � ������������
        EXPORT(CSTR_LineToTxt_Coord );
        break;
    case    CSTR_GETLENGTH:                        // ����� ������� ���������� ������
        EXPORT(CSTR_GetLength);
        break;
    case    CSTR_FNSAVE:       //
        EXPORT(CSTR_SaveRaster);
        break;
    case    CSTR_FNGETLIST :       //
        EXPORT(CSTR_GetComplist);
        break;
    case    CSTR_FNSETLIST :       //
        EXPORT(CSTR_SetComplist);
        break;
        case    CSTR_FNGETCOLLECTIONUNI :       // ����� ����� � ������
        EXPORT(CSTR_GetCollectionUni );
        break;
        case    CSTR_FNSTORECOLLECTIONUNI:       // ����� ����� � ������
        EXPORT(CSTR_StoreCollectionUni);
        break;
    case    CSTR_FNGETCONTAINER:    // ���� ����� ���������� ������
        EXPORT(CSTR_GetContainer);
        break;
    case     CSTR_FNSTORESCALE:     // ��������� ��� ���������������
        EXPORT(CSTR_StoreScale);
        break;
    case     CSTR_FNGETLINEFIRST:     // ���� ����� ������ ������ ������ ��������� � ������
        EXPORT(CSTR_GetLineFirst);
        break;
    case     CSTR_FNGETLINENEXT:      // ���� ����� ��������� ������ ������ ��������� � ������
        EXPORT(CSTR_GetLineNext);
        break;
    case    CSTR_FNGETRASTERLINE:     // ���� ����� ������ ������
        EXPORT(CSTR_GetRasterLine );
        break;
    case    CSTR_FNSETDUP:            // ��������� ����� ����������� ������
        EXPORT(CSTR_SetDup );
        break;
    case    CSTR_FNSETDUPEND:         // ��������� ����� ��������������� ����������� ������
        EXPORT(CSTR_SetDupEnd );
        break;
    case    CSTR_FNGETDUP:            // ���� ����� ����������� ������
        EXPORT(CSTR_GetDup );
        break;
    case    CSTR_FNGETDUPEND:         // ���� ����� ��������������� ����������� ������
        EXPORT(CSTR_GetDupEnd );
        break;
    case    CSTR_FNPACK:              // ������� ����������� ����������
        EXPORT(CSTR_PackLine);
        break;
    case    CSTR_FNUNPACK:            // ������������ ����������� ����������
        EXPORT(CSTR_UnpackLine);
        break;
    case      CSTR_FNSORTFRAGM:  // ������������� �� ����������
        EXPORT(CSTR_SortFragm);
        break;
    case      CSTR_FNFIRSTLINEFRAGM:  // ������ �� ���������
        EXPORT(CSTR_FirstLineFragm);
        break;
    case      CSTR_FNNEXTLINEFRAGM:  // ��������� �� ���������
        EXPORT(CSTR_NextLineFragm);
        break;
    case    CSTR_FNSTORECOMPORIGINAL: // ��������� ���������� (��� ������������� ����������)
        EXPORT(CSTR_StoreCompOriginal);
        break;
    case    CSTR_INSERTRASTERDOWN:     // �������� ������ ����
        EXPORT(CSTR_InsertRasterDown);
        break;
        case    CSTR_COPYRASTER:                        // ����������� ���������� ������
        EXPORT(CSTR_CopyRaster);
        break;
        case    CSTR_DELETELOOPS:                       // ������� �������� ����
        EXPORT(CSTR_DeleteLoops);
        break;
        case    CSTR_GETNEXTDOWN:                       // ������ �� ����� �����
        EXPORT(CSTR_GetNextDown);
        break;
        case    CSTR_GETPREVDOWN:                       // ������ �� ����� �����
        EXPORT(CSTR_GetPrevDown);
        break;
        case    CSTR_GETNEXTUP:                        // ������ �� ����� �����
        EXPORT(CSTR_GetNextUp);
        break;
        case    CSTR_GETPREVUP:                        // ������ �� ����� �����
        EXPORT(CSTR_GetPrevUp);
        break;
    default:
        wLowRC = CSTR_ERR_NOTIMPLEMENT;
                rc = FALSE;
        break;
        }
#undef EXPORT
return rc;
}

CSTR_FUNC(Bool32) CSTR_SetImportData(Word32 dwType, void * pData)
{

        wLowRC = CSTR_ERR_NO;
        switch(dwType)
        {
    case    CSTR_FNIMP_ALLOC:
        my_alloc=pData;
        break;
    case    CSTR_FNIMP_REALLOC:
        my_realloc=pData;
        break;
    case    CSTR_FNIMP_FREE:
        my_free=pData;
        break;

        default:
    wLowRC = CSTR_ERR_NOTIMPLEMENT;
                return FALSE;
        }
return TRUE;
}
