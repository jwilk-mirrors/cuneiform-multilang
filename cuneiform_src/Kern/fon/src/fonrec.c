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

// ���� ������� ��������� (< POROG_WEIGHT)
// ��� � ������ ������� ( < POROG_PROB )
// �������� ������ �������������
#define _ADD_STRAF_SMALL_
#define POROG_WEIGHT 2
#define POROG_PROB   200

// ����� �������� ������� ��������� -
// � �� � ��������� �� ��������
#define _GOOD_BIG_SMALL_

//#define _USEDIF_    // diskriminator test
//#define _NO_RECOG_
//#define _OLD_LEO_

#define ParolBase "ClBas"  // in ctb - cluster
#define POROG_GOOD 110     // if < POROG_GOOD - set unrecog

#include "fon.h"
#include "ctb.h"
#include "sfont.h"
#include "fonrec.h"

//#include "frhdefs.h"

#include <memory.h>
#include <string.h>
#include <stdlib.h>
/*#include <io.h>*/
#include <fcntl.h>
/*#include <sys\stat.h>*/

#include "compat_defs.h"


static int recog_raster(BYTE *r,WORD fullBytes,WORD w,WORD h,
				  FONBASE *fonba,RECRESULT *recres,
				  int maxNames,int inCTB,int col,int row);
static int DiskriminatorTest(void);

static char     alphabet[256];
static FONBASE  fonbase={0,0,0,0,0};

/////////////
 FON_FUNC(void)   FONDone(void)
 {
  void   EndBounds(void);
  void   EndScale(void);

	if(fonbase.start) free(fonbase.start);
	fonbase.start=NULL;
	fonbase.inBase=0;
	fonbase.countFont=0;
	fonbase.isFirst=FALSE;

	EndBounds();
	EndScale();
//  FONEndSnap();
//	CTB_done();       // need ???
 }
/////////////////
FONBASE *GetStaticFonbase(void)
{
	return &fonbase;
}
 //////////////
// ��������� ������ �����,
// ���������� next ��� ����������������� ��������
static void SetFirstNext(void)
{
 int i;
 welet *wel;
 Word16 tek[256];

	if(fonbase.isFirst)
		return;

	memset(&fonbase.first[0],0,256*sizeof(Word16));

	for(i=1,wel=fonbase.start;i<=fonbase.inBase;i++,wel++)
	{
      wel->next=0;
      if(wel->let < 0 || wel->let > 255)
		  continue;
	  if(fonbase.first[wel->let]==0)
          fonbase.first[wel->let]=i;
	  else
		  fonbase.start[tek[wel->let]-1].next=i;
      tek[wel->let]=i;
	}

    fonbase.isFirst=TRUE;
}
///////////////////
static int CheckFileClu(char *name)
{
access_tab act;
long ln;
int fh;
BYTE *mem=NULL;
welet *end;

 if((fh=open(name,O_RDONLY|O_BINARY))==-1) return 10;
 ln = filelength(fh);
 if( ln < sizeof(access_tab) )
 {
	 close(fh);
	 return 11;
 }

 lseek(fh,ln-sizeof(access_tab),SEEK_SET);
 if(read(fh,&act,sizeof(access_tab)) != sizeof(access_tab))
 {
	 close(fh);
	 return 12;
 }

 if( act.wlsize !=sizeof(welet) ||
     strcmp(act.sign,SIGNATURE) != 0
   )
 {
	 close(fh);
	 return 13;
 }

 lseek(fh,0,SEEK_SET);
 if((mem = malloc(ln)) == NULL) {close(fh); return -2;}
 if(read(fh,mem,ln) != ln)
	{free(mem); close(fh);
     return -3;
	}
 close(fh);

 memset(&fonbase,0,sizeof(FONBASE));
 fonbase.start = (welet *)mem;
 end   = (welet *)(mem + ln - sizeof(access_tab));
 fonbase.inBase= end-fonbase.start;
 fonbase.ace   = (access_tab *)end;
 return 0;
}
//////////////////
///////////////////////
// return == 0 - error
static int GetCTBasWelet(CTB_handle *CTBhandle,int num,welet *wel)
{
Word8     CTBdata[CTB_DATA_SIZE];
Word16   *pword16;
Int16    *pint16;
Word32   *pword32;


  if( CTB_read(CTBhandle,num,wel->raster,CTBdata) == FALSE )
	 return 0;

    wel->let    = CTBdata[3];    // in ASCII
	wel->w      = CTBdata[4];
	wel->h      = CTBdata[5];
	wel->weight = CTBdata[6];   // how many symbols make
	wel->porog  = CTBdata[7];   // threshold
	wel->mw     = CTBdata[8];   // medium width
	wel->mh     = CTBdata[9];   //        height
	wel->prob   = CTBdata[10];  // for CTB - probability
	wel->attr   = CTBdata[11];

	// now put words
    pword16=(Word16 *)(CTBdata+12);
	wel->fill   = pword16[0];
	wel->num    = pword16[1];
	wel->invalid= pword16[2];

	wel->valid  = CTBdata[18];   // for CTB - validity
	wel->kegl   = CTBdata[19];

	// now short int's
	pint16=(Int16 *)(CTBdata+20);
	wel->sr_col = pint16[0];
	wel->sr_row = pint16[1];

	// now dword
	pword32=(Word32 *)(CTBdata+24);
	wel->summa  = pword32[0];
	SetFields(wel->fields,(pword32+1));

	pint16=(Int16 *)(CTBdata+28+NFIELDDWORD*sizeof(Word32));
	wel->nInCTB = pint16[0];
	pword32=(Word32 *)(pint16+1);
	wel->tablColumn = pword32[0];

  return 1;
}
/////////////
static int LoadCTB(char *name)
{
 CTB_handle ctbhan;
 Word8 CTBdata[CTB_DATA_SIZE];
 welet *wel,*twel;
 int  num;
 int  i;
 Int16 *pint16;
 Word32 *pword32;

 // process CTB-file
 if( CTB_open(name,&ctbhan,"r") == FALSE )
 {	// MessageBox(GetActiveWindow(),name,"Error open CTB-base",MB_OK);
	 return -41;
 }

 // grey file ?
 if( (ctbhan.signums & CTB_GRAY) == 0 )
 {
	 CTB_close(&ctbhan);
//	 MessageBox(GetActiveWindow(),name,"Not grey base",MB_OK);
	 return -42;
 }

 if( CTB_read_global_data(&ctbhan,CTBdata) == 0 ||
	 memcmp(&CTBdata[1],ParolBase,5) != 0
	 )
 {
	 CTB_close(&ctbhan);
//	 MessageBox(GetActiveWindow(),name,"Not cluster CTB-base",MB_OK);
	 return -43;
 }

 num=CTB_volume(&ctbhan);

 if( (wel = (welet *)malloc(num*sizeof(welet))) == NULL)
	{CTB_close(&ctbhan); return -1;}

 for(i=0,twel=wel;i<num;i++,twel++)
 {
    if( GetCTBasWelet(&ctbhan,i,twel) == 0 )
		{CTB_close(&ctbhan); return -2;}
 }
 CTB_close(&ctbhan);

 // now to static
 memset(&fonbase,0,sizeof(FONBASE));
 pint16 =(Int16 *)(CTBdata+6);
 pword32=(Word32 *)(CTBdata+8);
 fonbase.start = wel;
 fonbase.inBase= num;
 fonbase.ace   = NULL;
 fonbase.countFont=*pint16;
// ���������� � ������� � �����
 SetFields(fonbase.fontFields[0],pword32);
 SetFields(fonbase.fontFields[1],(pword32+2));
 SetFields(fonbase.fontFields[2],(pword32+4));
 SetFields(fonbase.fontFields[3],(pword32+6));

 return num;
}
////////////////////
 FON_FUNC(Int32) FONInit(char *name)
 {
 int i,j;
 welet *wel;


 // was init - return old
 if(fonbase.start )
 {
	 if(!fonbase.isFirst)
		 SetFirstNext();
	 return fonbase.inBase;
 }

 //FONInitSnap(NULL);

 memset(&fonbase,0,sizeof(FONBASE));
 if(name==NULL)
	 return 0;

 if( (i=CheckFileClu(name)) < 0 )
	  return i;   // clu-file, but  no memory or s.o.

 if( i > 0 ) // not clu-file
   i=LoadCTB(name);

 if( i < 0 ) return i;

 for(i=0,j=0,wel=fonbase.start;i<fonbase.inBase;i++,wel++)
	 if( wel->attr & FON_CLU_SOLID ) j=i;
 fonbase.inBase=j+1; // not need not-solid for recognition !
 FONCutOffClusters(fonbase.start,(SINT)fonbase.inBase);

 SetFirstNext();

 {
  int AnalyzeTablColumn(welet *wel,int numWelet,int column);
  for(i=1;i<=32;i++)
	 fonbase.tablColumn[i-1]=AnalyzeTablColumn(fonbase.start,fonbase.inBase,i);
 }


#ifdef _USEDIF_
 DiskriminatorTest();
#endif

 return fonbase.inBase;
 }
//////////////////////////////////////////
#define POROG_TWIN 7
#define POROG_ALERT 180
#define POROG_WID_REJECT 3
#define POROG_HEI_REJECT 3

 // �������� ���� � ���� DWORD[2]
static Int32 GetBaseField(Int16 nField,FONBASE *fbase,Word32 *field)
 {
  if(nField <= 0 || nField > MAXFIELD)
             return 0;
  ClearFields(field);
  if( nField <= 32)
		field[0] = ((DWORD)1)<<(nField-1);
  else
	    field[1] = ((DWORD)1)<<(nField-33);
 return 1;
 }
 ///////////////
static void SetFonFlags(FonSpecInfo *specInfo,RecVersions *collection,RecRaster *recRast)
{
int i,j;
welet  *wel;
Word32 nField[NFIELDDWORD];
BOOL   isInField;

	if(!specInfo) return;
	if( !FONCheckItself((Word8)specInfo->nLet, specInfo->nInCTB,specInfo->col,specInfo->row) )
		return;

    if( collection->lnAltCnt > 1 )
	{
      if( collection->Alt[0].Prob - collection->Alt[1].Prob < POROG_TWIN )
          specInfo->wFonFlags |= FFC_FON_TWIN;

	  if(  collection->Alt[0].Prob == collection->Alt[1].Prob &&
			collection->Alt[1].Code == specInfo->nLet
		 )
		{
			RecAlt tmpAlt=collection->Alt[0];
            collection->Alt[0]=collection->Alt[1];
			collection->Alt[1]=tmpAlt;
		}
    }

	if( collection->lnAltCnt > 0 )
	{
		if( specInfo->nLet )
		{
		 if(collection->Alt[0].Code == specInfo->nLet)
          specInfo->wFonFlags |=FFC_FON_CONFIRMED;
		 else
          specInfo->wFonFlags |=FFC_FON_CHANGE;
		}

  	    if( collection->Alt[0].Prob < POROG_ALERT )
              specInfo->wFonFlags |=FFC_FON_ALERT;
		return;
	}

	if( specInfo->nLet == 0 )
		 return;

	// test reject
    // translate to base fields
	if( GetBaseField(specInfo->nFieldRow,&fonbase,nField)==0 ||
	    EmptyField(nField)
	   )
		  return;
	      // goto metkaAlert;

    isInField =FALSE;

    for(i=0,j=0,wel=fonbase.start;i<fonbase.inBase;i++,wel++)
	{
		if(!(wel->attr & FON_CLU_SOLID)) continue;

		if( wel->let != specInfo->nLet)  continue;

		// �� ������ ���� ?
		if( !IntersectFields(nField, wel->fields) )
		// �� ������ ������ ?
		{
			int k,all=MIN(fonbase.countFont,4);
			for(k=0;k<all;k++)
			{
               if( IntersectFields ( nField, fonbase.fontFields[k] ) &&
                   IntersectFields ( wel->fields, fonbase.fontFields[k] )
				 ) break;
			}
			if(k>=all)
				continue;
		}

		j=i+1;
           //  ���� �������� ?
		if( abs(recRast->lnPixWidth-wel->mw) > POROG_WID_REJECT ||
                abs(recRast->lnPixHeight-wel->mh) > POROG_HEI_REJECT )
             isInField=TRUE;
	}

    if( j == 0 )  // ��� ����� �����
	 return;

//metkaAlert:
	// ���� ���� ������� ?
	if( isInField )
		specInfo->wFonFlags |= FFC_FON_REJECT;
    else // if(!goodHeight)
	    specInfo->wFonFlags |= FFC_FON_ALERT;

	return;
}
///////////////
static void ReorderCollection(RecVersions *collection)
{
int i,j;
RecAlt tmpAlt;

  for(i=0;i<collection->lnAltCnt;i++)
  {
	  for(j=i+1;j<collection->lnAltCnt;j++)
	  {
		  if(collection->Alt[j].Prob >
			 collection->Alt[i].Prob  )
		  {
              tmpAlt=collection->Alt[i];
			  collection->Alt[i]=collection->Alt[j];
			  collection->Alt[j]=tmpAlt;
		  }
	  }
  }
  return;
}
///////////////////////////////
static void FineFONCollection(RecVersions *collection)
{
int i,j;
RecVersions ver;

  for(i=0;i<collection->lnAltCnt;i++)
        {
        collection->Alt[i].Method = REC_METHOD_FON;
        }

  ver = *collection;
  for(i=j=0;i<ver.lnAltCnt;i++)
  if( alphabet[ver.Alt[i].Code] )
        collection->Alt[j++] = ver.Alt[i];
  collection->lnAltCnt = j;
  return;
}
///////////////////////////////

FON_FUNC(Int32) FONRecogChar(RecRaster *recRast,RecVersions *collection,FonSpecInfo *specInfo)
{
int xbit  =recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
RECRESULT recres[REC_MAX_VERS];
Int32 nInCTB,nField;
Int16 col,row;
Int32 countRazmaz;

  memset(collection,0,sizeof(RecVersions));

#ifdef _OLD_LEO_
   nInCTB=(int)specInfo;
   nField=0;
   specInfo=NULL;
#else
  if(specInfo)
  {
	  nInCTB = specInfo->nInCTB;
	  nField = specInfo->nFieldRow;
	  specInfo->wFonFlags=0;
	  specInfo->nClust   =0;
	  col=specInfo->col;
	  row=specInfo->row;
	  countRazmaz = specInfo->countRazmaz;
  }
  else
  {
	  nInCTB=0;
	  nField=0;
	  col=row=-1024;
	  countRazmaz = 1;
  }
#endif

#ifdef _NO_RECOG_
  return 0;
#endif

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=RecogClu(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  recres,(SINT)REC_MAX_VERS, fonbase.start,fonbase.inBase,-1,
	  nInCTB,col,row, countRazmaz);

  if(ret < 0) return ret;
  collection->lnAltCnt=0;
  for(xbit=0;xbit<ret;xbit++)
   {
    // ����� ������ �� �����
    if( recres[xbit].prob < 40 )
		break;
    collection->lnAltCnt++;
    collection->Alt[xbit].Code=recres[xbit].name;
    collection->Alt[xbit].Prob=recres[xbit].prob;

#ifdef _ADD_STRAF_SMALL_
	{
     int nClus;
     if( (nClus=recres[xbit].nClust) >= 0  &&
		 nClus < fonbase.inBase &&
		 ( fonbase.start[nClus].weight < POROG_WEIGHT ||
		   fonbase.start[nClus].prob < POROG_PROB
		 ) &&
         collection->Alt[xbit].Prob>0
	    )
      collection->Alt[xbit].Prob--;
	}
#endif
   }
#ifdef _ADD_STRAF_SMALL_
  // ����� ��������� �������
  ReorderCollection(collection);
#endif
  FineFONCollection(collection);


  if(specInfo)
  {
	  // ��������� ������
	  SetFonFlags(specInfo,collection,recRast);

	  if( collection->lnAltCnt > 0)
	  {
      // ����� ������� ������� ������
	   for(xbit=0;xbit<ret;xbit++)
	   {
         if(collection->Alt[0].Code==recres[xbit].name)
		 {
           specInfo->nClust=recres[xbit].nClust+1;
		   break;
		 }
	   }
	  }
  }

  return collection->lnAltCnt;
}
///////////////////
FON_FUNC(Int32) FONRecogCharTiger(RecRaster *recRast,RecVersions *collection,FonSpecInfo *specInfo)
{
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
RECRESULT recres[REC_MAX_VERS];
Int32 nInCTB,nField;
Int16 col,row;

  memset(collection,0,sizeof(RecVersions));

  if(specInfo)
  {
	  nInCTB = specInfo->nInCTB;
	  nField = specInfo->nFieldRow;
	  specInfo->wFonFlags=0;
	  specInfo->nClust=0;
	  col=specInfo->col;
	  row=specInfo->row;
  }
  else
  {
	  nInCTB=0;
	  nField=0;
	  col=row=-1024;
  }

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=recog_raster(recRast->Raster,(WORD)bytesx,(WORD)xbit,(WORD)recRast->lnPixHeight,
				  &fonbase,recres,REC_MAX_VERS,nInCTB,col,row);

  if(ret <= 0) return 0;

  collection->lnAltCnt=ret;
  for(xbit=0;xbit<ret;xbit++)
  {
    collection->Alt[xbit].Code=recres[xbit].name;
    collection->Alt[xbit].Prob=recres[xbit].prob;

#ifdef _ADD_STRAF_SMALL_
	{
     int nClus;
     if( (nClus=recres[xbit].nClust) >= 0  &&
		 nClus < fonbase.inBase &&
		 ( fonbase.start[nClus].weight < POROG_WEIGHT ||
		   fonbase.start[nClus].prob < POROG_PROB
		 ) &&
         collection->Alt[xbit].Prob>0
	    )
      collection->Alt[xbit].Prob--;
	}
#endif
  }

#ifdef _ADD_STRAF_SMALL_
  // ����� ��������� �������
  ReorderCollection(collection);
#endif
  FineFONCollection(collection);


  if(specInfo)
  {
	  // ��������� ������
	  SetFonFlags(specInfo,collection,recRast);

	  if( collection->lnAltCnt > 0)
	  {
      // ����� ������� ������� ������
	   for(xbit=0;xbit<ret;xbit++)
	   {
         if(collection->Alt[0].Code==recres[xbit].name)
		 {
           specInfo->nClust=recres[xbit].nClust+1;
		   break;
		 }
	   }
	  }
  }

  return collection->lnAltCnt;
}
///////////////////
FON_FUNC(Bool32) FONCheckItself(Word8 let,Int32 num,Int16 col,Int16 row)
{
 welet *wel;
 int i;

 if(fonbase.start==NULL) return FALSE;
 for(i=0,wel=fonbase.start;i<fonbase.inBase;i++,wel++)
 {
	 //  itself
   if( !(wel->attr & FON_CLU_SOLID) )
	   continue;
   if( wel->invalid)
	   continue;
   if( wel->weight == 1 && (num == wel->nInCTB ||
//	   col==wel->sr_col && row==wel->sr_row
      abs(col-wel->sr_col)<=1 && abs(row-wel->sr_row)<=1 )
	 )
     return FALSE;
 }
 return TRUE;
}
//////////////////////////
//
//  return -2 - too big symbol
//         -1 - no such letter
//       >=0  - how many variants
//
FON_FUNC(Int32) FONTestChar(RecRaster *recRast,Word8 let,FonTestInfo *attr,Int16 nInCTB)
{
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
SINT CheckClu(BYTE *rast,SINT xbyte,SINT xbit,SINT yrow,
			  FONBASE *fbase,int let,FonTestInfo *attr,Int16 nInCTB);


  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return -2;

  ret=CheckClu(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  &fonbase,let,attr,nInCTB);

  return ret;
}
///////////////////
FON_FUNC(Int32) FONTestCharTiger(RecRaster *recRast,Word8 let,FonTestInfo *attr)
{
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
int CheckRaster(PBYTE r,WORD fullBytes,WORD w,WORD h,
			    int let,FonTestInfo *attr);


  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return -2;

  ret=CheckRaster(recRast->Raster,(WORD)bytesx,(WORD)xbit,(WORD)recRast->lnPixHeight,
				  let,attr);

  return ret;
}
///////////////////

FON_FUNC(Int32) FONSetAlphabet(char *coll)
{
  int i;
  welet *wel;

  memcpy(alphabet,coll,256); // Oleg : 01.07.1997 : copies of external alphabet

  if( fonbase.start )
  {
    for(i=0,wel=fonbase.start;i<fonbase.inBase;i++,wel++)
	{
       if( wel->let < 0 || wel->let > 255 )
		   continue;
	   if( alphabet[wel->let] )
		   wel->num=abs(wel->num);
	   else
		   wel->num=-abs(wel->num);
	}
  }

  return 1;
}
///////////////////
// return number of fonts
FON_FUNC(Int32) FONGetFontCount(void)
{
	if(fonbase.inBase <= 0 ||
       fonbase.start == NULL )
	   return -1;
	return fonbase.countFont;
}
/////////////////
// return number of distinct letters
FON_FUNC(Int32) FONFontInfo(FontInfo *fontinfo)
{
	int i,count;
	welet *wel;
	int k;

	memset(fontinfo,0,sizeof(FontInfo));
	if(fonbase.inBase <= 0) return 0;
	for(count=0,k=0,wel=fonbase.start;k<fonbase.inBase;wel++,k++)
	{
		if( !(wel->attr & FON_CLU_SOLID)  ) continue;
		if( wel->invalid ) continue;
		if( (i=wel->let) < 0 || i > 255 ) continue;

        fontinfo->numFont[i]++;

			// was same - for big russian may be small
		if(  fontinfo->count[i] > 0 )
		{

#ifndef _GOOD_BIG_SMALL_
			if( i >= 130 && i < 160 && i != 133 )
			{
				int better,j;
				j=i+(i<144?32:80);
				if( wel->weight <= fontinfo->count[j] &&
					wel->weight <= fontinfo->count[i] )
					  continue;
				if( fontinfo->count[j] <= 0) count++;
				if( fontinfo->count[i] >= fontinfo->count[j] )
					better=i;
				else better=j;
				if(wel->mh > fontinfo->height[better] ||
				   wel->mh == fontinfo->height[better] &&
				   wel->mw > fontinfo->width[better] )
					{
					 fontinfo->count[j]=fontinfo->count[better];
					 fontinfo->width[j]=fontinfo->width[better];
					 fontinfo->height[j]=fontinfo->height[better];
					 fontinfo->number[j]=fontinfo->number[better];
					}
				else
					{
					 fontinfo->count[i] =fontinfo->count[better];
					 fontinfo->width[i] =fontinfo->width[better];
					 fontinfo->height[i]=fontinfo->height[better];
					 fontinfo->number[i]=fontinfo->number[better];
					 i=j; // put here welet
					}
			}

			else
#endif
				if( wel->weight <= fontinfo->count[i] )
			 continue;
		}
		else count++;

		fontinfo->count[i] =wel->weight;
		fontinfo->width[i] =wel->mw;
		fontinfo->height[i]=wel->mh;
		fontinfo->number[i]=k;
	}

	return count;
}
////////////////////////////
// get information about num first symbols
// return number of symbols (<=num)
FON_FUNC(Int32) FONSizesInfo(SizesInfo *sizeinfo,int num)
{
	int i,j,count;
	welet *wel;

	if(fonbase.inBase <= 0) return 0;
	for(count=0,j=fonbase.inBase,wel=fonbase.start;j>0;wel++,j--)
	{
		if( !(wel->attr & FON_CLU_SOLID)  ) continue;
		if( wel->invalid ) continue;
		if( (i=wel->let) < 0 || i > 255 ) continue;

        sizeinfo[count].name =wel->let;
        sizeinfo[count].count=wel->weight;
		sizeinfo[count].width=wel->mw;
		sizeinfo[count].height=wel->mh;
		count++;
		if(count >= num) break;
	}

	return count;
}
////////////////////////////
//
//  Recognition itself
//
////////////////////////////////////////////
//static WORD  (*_cmp)(PBYTE r,WORD h,WORD w,welet * wl,INT xo,INT yo);

///////////////////////
SINT cmp0(PBYTE r,WORD fullByte,WORD w,WORD h,welet * wl,SINT xo,SINT yo)
{
LONG  n,ac,np;
PCHAR curr;
SINT i,j,jj;
BYTE  cbyte,cc;
SINT rbyte;
int avr;

 n=ac=np=0;
 avr = wl->summa/wl->fill;

 curr = wl->raster + WR_MAX_WIDTH*((WR_MAX_HEIGHT-h)/2) +
				  (WR_MAX_WIDTH-w)/2;
 curr += xo + WR_MAX_WIDTH * yo; // variation

 rbyte=(w+7)>>3;

 for(i=0; i < h;i++,curr += WR_MAX_WIDTH,r+=fullByte)
 {
  for(j=jj=0;j<rbyte;j++)
  {
   cbyte=r[j];
   if(cbyte == 0) {jj+=8;continue;}
   for(cc=128;cc;cc>>=1,jj++)
   {
    if(cbyte & cc)
    {
     ac+= curr[jj];
     if( curr[jj] < 0 )
     {     n  += curr[jj];
           np++;
     }
	} // end if
   } // end cc
  }  // end j
 }   // end i

  n = abs(n) - np * avr; // all points with dist 1
  n *= n;
  n /= avr;
  ac -= n; // progressive penalty
  return (SINT) (( ac <= 0 ) ? 0 : (ac * 255) / wl->summa);
}
///////////////////////
// _cmp = cmpMMX  - was in Tiger
static SINT  (*_cmp)(PBYTE r,WORD fullw,WORD w,WORD h,welet * wl,SINT xo,SINT yo)=cmp0;
///////////
WORD cmp(PBYTE r,WORD fullwb,WORD w,WORD h,welet * wl)
{
 WORD best,east,west,north,south,center;
 int   lbest; // local best

 best=east=west=north=south=center=lbest=0;

 best = (_cmp)(r,fullwb,w,h,wl, 0, 0);
// wl->work = pack_coord( 0, 0);
 center = best;
 if(best >= 240) return best;

 if(best == 0) return 0;  // test !  < 20 ?

 if((south = (_cmp)(r,fullwb,w,h,wl, 0, 1)) > best )
    best = south;
 if((north = (_cmp)(r,fullwb,w,h,wl, 0,-1)) > best )
   best = north;
 if((east = (_cmp)(r,fullwb,w,h,wl, 1, 0)) > best )
    best = east;
 if((west = (_cmp)(r,fullwb,w,h,wl,-1, 0)) > best )
    best = west;

 if( north >= center && east >= center )
 {
  if((lbest = (_cmp)(r,fullwb,w,h,wl,1,-1)) > best )
    best = lbest;
 }
 else if(east >= center && south >= center )
 {
  if((lbest = (_cmp)(r,fullwb,w,h,wl,1, 1)) > best )
    best = lbest;
 }
 else if(south >= center && west >= center)
 {
  if((lbest = (_cmp)(r,fullwb,w,h,wl,-1, 1)) > best )
    best = lbest;
 }
 else if( west >= center && north >= center)
 {
  if((lbest = (_cmp)(r,fullwb,w,h,wl,-1,-1)) > best )
    best = lbest;
 }

 return best;
}

///////////////////////////
int recogWelet(PBYTE r,WORD fullRow,WORD w,WORD h,welet * wl)
{
	 if(!(wl->attr & FON_CLU_SOLID))
       return 0; // suspect cluster
//   if(! can_compare(wl))          return ver; // prevent rec letter by itself
    if(wl->invalid)
       return 0; // suspect cluster
//   if(shadow_clu(wl))		  return ver; //
//   if( h >= WR_MAX_HEIGHT-1 ) return 0;
//   if( w >= WR_MAX_WIDTH-1 )  return 0;

   if(abs(wl->mh-h)*5 > h ) return 0; // check for height likeness
//   if(abs(wl->w-w) >= MIN(wl->w,w)/2) return 0;
//   if(abs(wl->mw-w)*3 >= MIN(wl->mw,w) ) return 0;
   if(abs(wl->mw-w)*3 >= wl->mw ) return 0;

   return cmp(r,fullRow,w,h,wl);
}
///////////////////////
///////
int AddVersion(RECRESULT *recres,BYTE let,BYTE rec,int nClust,
			   int num,int maxNames)
{
int j,k;

   for(j=0;j<num;j++ )
	 {
	   if( recres[j].name == let	)
	   {
        if( recres[j].prob < rec )
        {
         recres[j].prob   = rec;
         recres[j].nClust = nClust;
        }
		  return num;
		} // was better the same

	   if(rec > recres[j].prob) break;
	 }

   if( j>=maxNames ) return num;

	  // was such name - to new position
   for(k=j+1;k<num;k++ )
	 {
      if( recres[k].name == let )
	   {   memmove(recres+j+1,recres+j,sizeof(RECRESULT)*(k-j));
			 recres[j].name=let;
          recres[j].prob=rec;
          recres[j].nClust=nClust;
          return num;
	   }
	 }

   if(num<maxNames) num++;
   if( j < num-1)
    memmove(recres+j+1,recres+j,sizeof(RECRESULT)*(num-j-1));

   recres[j].name=let;
   recres[j].prob=rec;
   recres[j].nClust=nClust;

   return num;
}
//////////////
int recog_raster(PBYTE r,WORD fullBytes,WORD w,WORD h,
				  FONBASE *fonba,RECRESULT *recres,
				  int maxNames,int inCTB,int col,int row)
{
int i;
welet *wel;
int num=0;
int rec;

 //memset(probs,0,maxNames);

 for(i=0,wel=fonba->start,num=0;i<fonba->inBase;i++,wel++)
 {
	 // don't recog itself
   if( wel->weight == 1 &&
	   (wel->nInCTB == inCTB ||
//	    wel->sr_col == col && wel->sr_row==row
		abs(col-wel->sr_col)<=1 && abs(row-wel->sr_row)<=1 )
	)
	   continue;
   if( wel->num < 0 )  // �� �� ������ ��������
	   continue;
   if( (rec=recogWelet(r,fullBytes,w,h,wel)) < POROG_GOOD )
      continue;

   num=AddVersion(recres,(BYTE)wel->let,(BYTE)rec,i,
			    num, maxNames);
 }
 return num;
}
/////////////////////
int CheckRaster(PBYTE r,WORD fullBytes,WORD w,WORD h,
				int let,FonTestInfo *attr)
{
int i;
int num=0;
int rec;
int nAlt=0;
welet *wel;
int AddTestAlt(Word8 prob,int numAlt,FonTestInfo *attr,
			   welet *wel,int nClust);

 if( let<0 || let > 255)
	 return -1;

 i=fonbase.first[let];
 for(num=0;i>0; i=wel->next)
 {
   wel=fonbase.start+i-1;

   if( wel->let != let ) continue;
   if( wel->invalid ) continue;
   if( !(wel->attr & FON_CLU_SOLID)  ) continue;

   num++;
   if( (rec=recogWelet(r,fullBytes,w,h,wel)) <= POROG_GOOD )
	   continue;
   nAlt = AddTestAlt( (Word8)rec,nAlt,attr,wel,i);
 }
 if( num <= 0 ) return -1;
 return nAlt;
}
/////////////////////
/*
static WORD id=1971;
welet * wl;
WORD nvers=0,nrec;
BYTE index[16],*ndx;
PWORD lp;


 memset(answ,0,sizeof(Version)*(MAXVERS+1));
// {Valdemar classic version
 id++; // var to prevent recognition by the same cluster
 memset(index,255,sizeof(index));
#ifdef STANDALONE
 lp=init_fac(r,h,w);
#else
 lp = lpool+1;
#endif
  get_index(lp,index);
for(nvers=nrec=0,ndx = index; *ndx != 255 ; ndx++)
{
 for(wl=first_clu(*ndx); wl != NULL; wl=next_clu())
 {
   if(wl->work1 == id) continue;
   recv(r,h,w,wl,answ); nrec++;
   wl->work1 = id;
 }
 nvers = sort_versi(answ);
}
 nvers = cut_vers(answ);

 if(nvers==0 || answ[0].prob < PROB_OK )
 {
  DWORD i,nindexes;
  BYTE like[16];
  for(nindexes=ndx-index,i=0;i<nindexes;i++)
  {
   get_most_likely(index[i],like); // get all close indexes
    for(ndx=like;*ndx != 255;ndx++)
     for(wl=first_clu(*ndx); wl != NULL; wl=next_clu())
     {
      if(wl->work1 == id) continue;
      recv(r,h,w,wl,answ);     nrec++;
      wl->work1 = id;
     }
  }
  nvers = sort_versi(answ);
 } // if nothing were recognized

 if(nrec == 0  )//|| answ[0].prob < 100 )  // <100 - on cun 1-100 more 3 errors
 // we can't seek proper patterns. Try linear seek
     for(wl=first_clu(ALL_CELLS); wl != NULL; wl=next_clu())
     {
      if(wl->work1 == id) continue;
	  recv(r,h,w,wl,answ);     nrec++;
     }

 nvers = sort_versi(answ);
 nvers = cut_vers(answ);

 return nvers;
}
*/
// end Valdemar }
/////////////////////////////
///////////////////////////////
FON_FUNC(Int32) FONCompareRasterCluster(RecRaster *recRast,int numWelet,
										int movx,int movy)
{
int CompareCluster(BYTE *rast,int xbyte,int xbit,int yrow,welet *wel,
				   int movex,int movey);
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  if(fonbase.inBase <= 0) return 0;
  if(!fonbase.start) return 0;

  if(numWelet < 0 || numWelet >= fonbase.inBase) return -20;

  ret=CompareCluster(recRast->Raster,bytesx,xbit,recRast->lnPixHeight,
	   fonbase.start+numWelet,movx,movy);
  return ret;
}
///////////////////
//
//   [in] name   - from cluster with name == name (if name > 0)
//   [in] number - & order number >= number
//   [in] porog -  & use percent threshold  (0-100 ) (0 - default)
//   [out]recRaster - make black/white raster, put to recRaster
//
//   Return  >=0 - actual cluster number
//      < 0 - error
//       -10 - no base loaded
//       -20 - invalid number  (<0 or > actual number of clusters)
//       -21 - invalid name    (<0 or > 255)
//       -22 - no correct cluster
//
FON_FUNC(Int32) FONGetClusterAsBW(Int32 *name,Int32 number,Int32 porog,RecRaster *recRast)
{
 int xbit,yrow;
 int bytesx;
 CHAR *rr;
 int  startx;
 int  starty;
 int  i,j;
 welet *wel;
 BYTE *rr1,maska;


  if( fonbase.inBase <= 0 ||
	  fonbase.start == NULL  ) return -10;
  if(number < 0 || number >= fonbase.inBase )
	  return -20;
  if(name && ( *name < 0 || *name > 255) )
	  return -21;

  // looking for correct cluster
  if(name && *name > 0)
  {
   for(wel=fonbase.start+number;number<fonbase.inBase;number++,wel++)
     if( wel->let == *name ) break;
   if( number >= fonbase.inBase ) return -22;
  }
  else
  {wel=fonbase.start+number;
   if(name) *name=wel->let;
  }

  startx=(WR_MAX_WIDTH-wel->w)/2;
  starty=(WR_MAX_HEIGHT-wel->h)/2;
  xbit=wel->w;
  yrow=wel->h;
  rr=wel->raster+starty*WR_MAX_WIDTH+startx;

  recRast->lnPixWidth=recRast->lnPixHeight=0;
  recRast->lnRasterBufSize=REC_MAX_RASTER_SIZE;

  if(porog <= 0) porog=0;
  else porog=(porog*wel->weight)/100;
  if(porog >= wel->weight) porog=wel->weight-1;

   // need new sizes  ?
  if(porog > wel->porog)
  {
   int  fx=xbit;  // first x
   int  lx=0;   // last  x
   int  fy=yrow;  // first y
   int  ly=0;   // last  y


   // get new picture bounds from weighted
   for(i=0;i<yrow;i++,rr+=WR_MAX_WIDTH)
	 {
	  for(j=0;j<xbit;j++)
		{
		  if(rr[j] <= porog)   	  continue;
			 // check new bounds
		  if(j<fx) fx=j;
		  if(j>lx) lx=j;
		  if(i<fy) fy=i;
		  if(i>ly) ly=i;
		}
	  }
     xbit=lx-fx+1;
	 yrow=ly-fy+1;
	 if(xbit <= 0 || yrow <= 0 ) return number;
	 rr=wel->raster+(starty+fy)*WR_MAX_WIDTH+startx+fx;
	}

  bytesx=((xbit+63)/64)*8;
  recRast->lnPixWidth =xbit;
  recRast->lnPixHeight=yrow;

  memset(recRast->Raster,0,bytesx*yrow);

  for(rr1=recRast->Raster;yrow;yrow--,rr+=WR_MAX_WIDTH,rr1+=bytesx)
  {
	  for(j=0,maska=128;j<xbit;j++,maska>>=1)
	  { if(maska==0) maska=128;
        if(rr[j] > porog ) rr1[j>>3]|=maska;
	  }
  }
  return number;
}
///////////////
// test diskriminators
#ifdef _USEDIF_

#include "dif.h"
static int DiskriminatorTest(void)
{
RecRaster myRaster;
welet *wel;
int  i;
BOOL rett;
RecVersions collect;

  for(i=0,wel=fonbase.start; i< fonbase.inBase ; i++)
  {
		if( FONGetClusterAsBW(NULL,i,0,&myRaster) < 0 )
			continue;

	    rett=DIFInit( &myRaster, TRUE, FALSE);
        if(rett==FALSE)
			continue;

		memset(&collect,0,sizeof(RecVersions));
		collect.lnAltCnt=1;
		collect.Alt[0].Code=(BYTE)wel->let;
		collect.Alt[0].Prob=255;
		rett = DIFPenaltyChar(&myRaster,&collect);
		if(rett==FALSE)
			 continue;

		if(collect.Alt[0].Prob < 250)  // ??
			wel->solid=0;
  }

  return 0;
}
//////////
#endif
///////////////////////////////
// temporary ? :
//  recBounds  - massiv REC_MAX_VERS*recRast->lnPixHeight;
//
FON_FUNC(Int32) FONRecogCharBound(RecRaster *recRast,RecVersions *collection,
								  Int32 *recBounds)
{
SINT RecogCluBound(BYTE *rast,SINT xbyte,SINT xbit,SINT yyrow,BYTE *names,
			 BYTE *probs,SINT maxNames,welet *wl,int numWel,
			 int *bounds,int countRazmaz);
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
BYTE names[REC_MAX_VERS];
BYTE probs[REC_MAX_VERS];

  memset(collection,0,sizeof(RecVersions));

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=RecogCluBound(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  names,probs,(SINT)REC_MAX_VERS, fonbase.start,fonbase.inBase,
	  recBounds, 1 );

  if(ret <= 0) return 0;

  collection->lnAltCnt=ret;
  for(xbit=0;xbit<ret;xbit++)
  {
    collection->Alt[xbit].Code=names[xbit];
    collection->Alt[xbit].Prob=probs[xbit];
  }
  FineFONCollection(collection);

  return collection->lnAltCnt;
}
///////////////////
FON_FUNC(Int32) FONRecogKley(RecRaster *recRast,RecVersions *collection)
{
int KleyRecog(BYTE *inBuf,int xbyte,int xbit, int yrow,
					 welet *wl,int numWel,int porog,
					 BYTE *names,BYTE *probs,int maxNames);
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
BYTE names[REC_MAX_VERS];
BYTE probs[REC_MAX_VERS];

  memset(collection,0,sizeof(RecVersions));

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=KleyRecog(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  fonbase.start,fonbase.inBase, 20, // porog
	  names,probs,(SINT)REC_MAX_VERS );

  if(ret <= 0) return 0;

  collection->lnAltCnt=ret;
  for(xbit=0;xbit<ret;xbit++)
  {
    collection->Alt[xbit].Code=names[xbit];
    collection->Alt[xbit].Prob=probs[xbit];
  }
//  FineFONCollection(collection);

  return collection->lnAltCnt;
}
///////////////////
//
// added 1.10.98
 // returns count (N) => numbers [1..N] can be used as indexes
FON_FUNC(Int32) FONGetClustCount(void)
{
  if(fonbase.start)
     return fonbase.inBase;
  return -1;
}
///////////////////
FON_FUNC(Int32) FONGetClustInfo( ClustInfo *clustinfo,
                                    int num   // in range [1..FONGetClustCount(void)]
                                   )
{
   welet *wel;
   Word8 let=clustinfo->let;

   memset(clustinfo,0,sizeof(ClustInfo));

   if(fonbase.start == NULL ||
      num <= 0              ||
      num > fonbase.inBase )
      return -1;

   wel=fonbase.start+(num-1);

   if(let)
   {
     for(;num<=fonbase.inBase;num++,wel++)
		 if( wel->let == let )
			 break;
     if( num > fonbase.inBase )
		 return 0;
   }

   // fill - what need ...
   clustinfo->let    = wel->let;
   clustinfo->weight = wel->weight;
   clustinfo->prob   = wel->prob;
   clustinfo->valid  = (Word8)wel->valid;
   clustinfo->mw     = wel->mw;
   clustinfo->mh     = wel->mh;
   clustinfo->kegl   = wel->kegl;

   clustinfo->attr = CTB_PRINT_FONT;
   if(wel->attr & FON_CLU_ITALIC)
	   clustinfo->attr|=CTB_PRINT_ITALIC;
   if(wel->attr & FON_CLU_SERIF)
	   clustinfo->attr|=CTB_PRINT_SERIFIC;
   if(wel->attr & FON_CLU_BOLD)
	   clustinfo->attr|=CTB_PRINT_BOLD;
   if(wel->attr & FON_CLU_GELV)
	   clustinfo->attr|=CTB_PRINT_GELV;
   if(wel->attr & FON_CLU_NARROW)
	   clustinfo->attr|=CTB_PRINT_NARROW;

   return num;
}
///////////////////
FON_FUNC(Int32) FONGetNearestClusters( RecRaster* recRast,  // symbol
                                        ClustAlt result[],   // result table of K nearest klusters
                                        int nClustAltLen,    // length of table
                                        int nDistLevel       // don't report about clusters with distance more than nDistLevel
                                        )
{

int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
RECRESULT recres[32]; // really 4


  if(nClustAltLen > 32)
         nClustAltLen = 32;

  memset(result,0,sizeof(ClustAlt)*nClustAltLen);

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=RecogClu(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  recres,(SINT)nClustAltLen,
     fonbase.start,fonbase.inBase,
     nDistLevel,0,-1024,-1024,1);

//  ret=recog_raster(recRast->Raster,(WORD)bytesx,(WORD)xbit,(WORD)recRast->lnPixHeight,
//				  &fonbase,recres,nClustAltLen);

  if(ret <= 0) return 0;

  for(xbit=0;xbit<ret;xbit++)
  {
    result[xbit].nClust=recres[xbit].nClust+1;
    result[xbit].nDist =255-recres[xbit].prob;
  }

//  FineFONCollection(collection);

  return ret;
}
////////////////////////
//
// test clusters - compare clusters
//
// �������� - ���� �� ������� ����� ��������� � �������
// name1 & name2
//
//  return < 0 - error
//   else 0 - 255  (255 - the same )
//
FON_FUNC(Int32) FONCompareClustNames(Int32 name1,Int32 name2)
{
	if(fonbase.start == NULL ||
       fonbase.inBase <= 0 )
      return -10;

	return TestClustNames(fonbase.start,
		fonbase.inBase, name1, name2 );
}
///////////////
//
// ����� ��������, ������� �� ������� � �������� �������
// ������ - c 1
//
FON_FUNC(Int32) FONGetSameClust(Int32 num,RecVersions *collect)
{
 int i;

   if(fonbase.start == NULL ||
      num <= 0              ||
      num > fonbase.inBase )
      return -10;

   i=GetNearestClusters (num-1, fonbase.start,fonbase.inBase ,
	    collect );

   return i;
}
///////////////////
FON_FUNC(Int32) FONSetClusterInvalid(Int32 num)
{
 welet *wel;

	if(fonbase.start == NULL ||
      num <= 0              ||
      num > fonbase.inBase )
      return 0;

	wel=fonbase.start+num-1;
	wel->invalid=-1;
	wel->attr &= ~FON_CLU_SOLID;

	return 1;
}
//////////////////
int SetTablDigitColumn(FONBASE *fonbase,int column,int onlyDigits)
{
	int i;
	int numDigCol;  // digits in column
	int numOther;
	Word8 isDigit[10];
	welet *wel;

    if(column <=0 || column > 32)
		return 0;
    column=1<<(column-1);

	memset(isDigit,0,10);
	for(wel=fonbase->start,i=0;i<fonbase->inBase;wel++,i++)
	{
      if( wel->let <'0' || wel->let >'9')
		  continue;
      if( wel->tablColumn & column )
       isDigit[wel->let-'0']=1;
	}

	for(i=0,numDigCol=numOther=0;i< 10;i++)
	{
		if(isDigit[i])
			numDigCol++;
		else if(fonbase->first[i+'0'])
			numOther++;
	}

	if( numDigCol <= numOther )
		return 0;

	for(wel=fonbase->start,i=0;i<fonbase->inBase;wel++,i++)
	{
		// not our column
      if( !(wel->tablColumn & column) && wel->let != '+')
	  {
		  if( wel->let < '0' || wel->let >'9'
			  //  - old version  || isDigit[wel->let-'0']
			 )
		  {
			  wel->num=-abs(wel->num);
			  continue;
		  }
	  }

      if( onlyDigits && wel->let != '+' && (wel->let <'0' || wel->let >'9'))
		  {
			  wel->num=-abs(wel->num);
			  continue;
		  }

      wel->num=abs(wel->num);
      alphabet[wel->let] = 1;
	}

	return 1;
}
////////////
FON_FUNC(Int32) FONSetAlphaColumn(Int32 column,Bool32 onlyDigits)
{

	if(fonbase.start == NULL )
		return 0;
	if( column <= 0 || column > 32)
		return 0;
	if( fonbase.tablColumn[column-1] != FONDigitColumn &&
		fonbase.tablColumn[column-1] != FONDigitFuzzyColumn
	   )
	   return 0;

	if( fonbase.tablColumn[column-1] == FONDigitColumn )
		onlyDigits=TRUE;

	return SetTablDigitColumn(&fonbase,column, onlyDigits);
}
//////////////////
FON_FUNC(Int32) FONRecogOkr(RecRaster *recRast,RecVersions *collection,FonSpecInfo *specInfo,Int32 okr,Int32 proc)
{
int xbit  =recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
RECRESULT recres[REC_MAX_VERS];
Int32 nInCTB,nField;
Int16 col,row;
int RecogCluOkr(BYTE *rast,SINT xbyte,SINT xbit,SINT yrow,
              RECRESULT *recres, SINT maxNames,
              welet *wl,int numWel,
              int porog ,int nInCTB, Int16 col, Int16 row,
			  int okr, int proc);

  memset(collection,0,sizeof(RecVersions));

  if(specInfo)
  {
	  nInCTB = specInfo->nInCTB;
	  nField = specInfo->nFieldRow;
	  specInfo->wFonFlags=0;
	  specInfo->nClust   =0;
	  col=specInfo->col;
	  row=specInfo->row;
  }
  else
  {
	  nInCTB=0;
	  nField=0;
	  col=row=-1024;
  }

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=RecogCluOkr(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  recres,(SINT)REC_MAX_VERS, fonbase.start,fonbase.inBase,-1,
	  nInCTB,col,row,okr,proc );

  if(ret < 0) return ret;
  collection->lnAltCnt=0;
  for(xbit=0;xbit<ret;xbit++)
   {
    // ����� ������ �� �����
    if( recres[xbit].prob < 40 )
		break;
    collection->lnAltCnt++;
    collection->Alt[xbit].Code=recres[xbit].name;
    collection->Alt[xbit].Prob=recres[xbit].prob;

#ifdef _ADD_STRAF_SMALL_
	{
     int nClus;
     if( (nClus=recres[xbit].nClust) >= 0  &&
		 nClus < fonbase.inBase &&
		 ( fonbase.start[nClus].weight < POROG_WEIGHT ||
		   fonbase.start[nClus].prob < POROG_PROB
		 ) &&
         collection->Alt[xbit].Prob>0
	    )
      collection->Alt[xbit].Prob--;
	}
#endif
   }
#ifdef _ADD_STRAF_SMALL_
  // ����� ��������� �������
  ReorderCollection(collection);
#endif
  FineFONCollection(collection);


  if(specInfo)
  {
	  // ��������� ������
	  SetFonFlags(specInfo,collection,recRast);

	  if( collection->lnAltCnt > 0)
	  {
      // ����� ������� ������� ������
	   for(xbit=0;xbit<ret;xbit++)
	   {
         if(collection->Alt[0].Code==recres[xbit].name)
		 {
           specInfo->nClust=recres[xbit].nClust+1;
		   break;
		 }
	   }
	  }
  }

  return collection->lnAltCnt;
}
///////////////////
FON_FUNC(Int32) FONCompareOkrRasterCluster(RecRaster *recRast,int numWelet,
										int movx,int movy,int okr,int proc,
										int *dist1,int *dist2)
{
int xbit=recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
int CompareClusterOkr(BYTE *rast,int xbyte,int xbit,int yrow,welet *wel,
				   int movex,int movey,int okr,int proc,
				   int *dist1,int *dist2);

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  if(fonbase.inBase <= 0) return 0;
  if(!fonbase.start) return 0;

  if(numWelet < 0 || numWelet >= fonbase.inBase) return -20;

  ret=CompareClusterOkr(recRast->Raster,bytesx,xbit,recRast->lnPixHeight,
	   fonbase.start+numWelet,movx,movy,okr,proc,dist1,dist2);
  return ret;
}
///////////////////
FON_FUNC(Int32) FONRecogInner(RecRaster *recRast,RecVersions *collection,FonSpecInfo *specInfo)
{
int xbit  =recRast->lnPixWidth;
int bytesx=((xbit+63)/64)*8;
int ret;
RECRESULT recres[REC_MAX_VERS];
Int32 nInCTB,nField;
Int16 col,row;
int RecogCluInner(BYTE *rast,SINT xbyte,SINT xbit,SINT yrow,
              RECRESULT *recres, SINT maxNames,
              welet *wl,int numWel,
              int nInCTB, Int16 *col, Int16 *row);

  memset(collection,0,sizeof(RecVersions));

  if(specInfo)
  {
	  nInCTB = specInfo->nInCTB;
	  nField = specInfo->nFieldRow;
	  specInfo->wFonFlags=0;
	  specInfo->nClust   =0;
	  col=specInfo->w;
	  row=specInfo->h;
  }
  else
  {
	  nInCTB=0;
	  nField=0;
	  col=row=-1024;
  }

  if( recRast->lnPixHeight > WR_MAX_HEIGHT-2 ||
	  xbit > WR_MAX_WIDTH -2 )
	  return 0;

  ret=RecogCluInner(recRast->Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast->lnPixHeight,
	  recres,(SINT)REC_MAX_VERS, fonbase.start,fonbase.inBase,
	  nInCTB,&col,&row );

  if(ret < 0)
	  return ret;

  collection->lnAltCnt=0;
  for(xbit=0;xbit<ret;xbit++)
   {
    collection->lnAltCnt++;
    collection->Alt[xbit].Code=recres[xbit].name;
    collection->Alt[xbit].Prob=recres[xbit].prob;
   }

  FineFONCollection(collection);

  if(specInfo)
  {
	  // ��������� ������
	  SetFonFlags(specInfo,collection,recRast);

	  if( collection->lnAltCnt > 0)
	  {
      // ����� ������� ������� ������
	   for(xbit=0;xbit<ret;xbit++)
	   {
         if(collection->Alt[0].Code==recres[xbit].name)
		 {
           specInfo->nClust=recres[xbit].nClust+1;
		   break;
		 }
	   }

	   specInfo->w  = fonbase.start[specInfo->nClust-1].mw;
	   specInfo->h  = fonbase.start[specInfo->nClust-1].mh;

	   specInfo->col=recRast->lnPixWidth/2-specInfo->w/2-col;
       specInfo->row=recRast->lnPixHeight/2-specInfo->h/2-row;
	  }

  }

  return collection->lnAltCnt;
}
///////////////////
