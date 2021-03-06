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

#include "c_types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "leo.h"
#include "ctb.h"
#include "sfont.h"
#include "fonrec.h"
#include "clu_lang.h"

#define POROG_TEST 5
#define POROG_SAME 3
#define POROG_SMES 8

extern const	int  Num11[256];
extern InfoCluster infoClusterStat[MAXWEICLUS];
extern int countCluster[256]; // how many clust

Bool32 BadCluster(InfoCluster *infoC);
void GetClusterStatistic(int numSymbol,int numCluster,Nraster_header *rh,
						SINT *nClus,InfoCluster *infoC,int *countC,
						BYTE *metkaGood,BYTE *metkaValid,BOOL addLingvo);

#define GoodCluster(infoC) (infoC.valid & LEO_VALID_LINGVO )

//  hvosty obnuleny v Razmaz !
static int GetVes(Nraster_header *rh)
{
	int i=rh->xbyte*rh->h;
	BYTE *buf=rh->pHau;
	int summa;

	for(summa=0;i>0;i--,buf++)
		summa+=Num11[*buf];

    return summa;
}
///////////////
static int GetAddField(int num,Word32 *ifield,int let,
						  int numCluster,InfoCluster *infoC,
						  Word32 *addField)
{
	Word32 field[NFIELDDWORD];
	int i;

	SetFields(field,ifield);
	ClearFields(addField);
	for(i=0;i<numCluster;i++)
	{
		if( i == num ) continue;
        if( !GoodCluster(infoC[i]) )   continue;
		if( infoC[i].let != let ) continue;
		if( IntersectFields(field,infoC[i].fields) )
			AddFields(field , infoC[i].fields)
		else
            AddFields(addField,infoC[i].fields)
	}

	if( IntersectFields(field, addField) )
		return 0;
	return 1;
}
//////////////
static Bool32 isAddField(Word8 name,InfoCluster *infoC,int numCluster,Word32 *addField)
{
 int i;

	for(i=0;i<numCluster;i++)
	{
		if( BadCluster(infoC+i) ) continue;

		if( infoC[i].let != name )
			continue;

		if( IntersectFields(infoC[i].fields,addField) )
			return TRUE;
	}

	return FALSE;
}
///////////
// ���� - ��� ��� �� �����
// � ��������� - ��� ������
static int smes[256];
static int uniqal[256];
static int GetSameField(Word32 *field,Word32 *addField,int let,
						  int numCluster,InfoCluster *infoC,
						  int *numSmes,
						  Word32 *sameField)
{
	int i,numSame,nSmes=0;
	int k;

	SetFields(sameField,field);
	for(i=0,numSame=1;i<numCluster;i++)
	{
		if( BadCluster(infoC+i) ) continue;

		if( infoC[i].let == let ) continue;
		if( infoC[i].let< 0 || infoC[i].let> 255)
			continue;

        // if(!strchr(testLet,let))  continue;

		// ��� �������� � ������ ?
		if( infoC[i].good )
		{
			smes[infoC[i].let]=-1;
			uniqal[infoC[i].let]=i+1;
			continue;
		}


		// ���� ��������� �������
		if( uniqal[infoC[i].let] ) continue;

		// undefined ?
        if( EmptyField(infoC[i].fields) ) continue;

		if( !IntersectFields(field,infoC[i].fields) )
			continue;

		if( IntersectFields(addField,infoC[i].fields)  )
		{
			// ������� ������ ?
			if( infoC[i].count < POROG_TEST)
		      continue;

			// ����������� � ������
			k=smes[infoC[i].let]-1;
		    if( k < 0 || infoC[k].count < infoC[i].count )
		     smes[infoC[i].let]=i+1;
			nSmes++;
			continue;
		}

		smes[infoC[i].let] = -1;
		k=uniqal[infoC[i].let]-1;
		if( k < 0 || infoC[k].count < infoC[i].count )
		     uniqal[infoC[i].let]=i+1;

        AddFields(sameField, infoC[i].fields);

		if( infoC[uniqal[infoC[i].let]-1].count >= POROG_TEST &&
			isAddField((Word8)infoC[i].let,infoC,numCluster,addField)
		  )
			numSame++;
	}

	if( nSmes == 0)
		return 0;

	if( numSame <= 3 )
		return 0;

	*numSmes= numSame;

	return 1;
}
//////////////
static void FillInfo(InfoCluster *infoC, Nraster_header *rh, int i)
{

	MakeDWORDField(rh->nField,infoC->fields);

    infoC->mw+=rh->w;
    infoC->mh+=rh->h;
	if( infoC->count == 0)
         infoC->first=i;
	infoC->count++;
	infoC->prob   = MAX(infoC->prob,rh->prob);
	infoC->valid |= rh->valid;
}
//////////////
// i - new, j - old
static Bool32 SameSizes(InfoCluster *infoC,int i,int j,
						int *sumNew,int *wNew,int *hNew,
						int *sumOld,int *wOld,int *hOld
						)
{ // middle value
	   int c1,c12,c2,c22;

	   c1=infoC[i].count;
	   c2=infoC[j].count;
	   if( c1 == 0 || c2 == 0)
		   return TRUE;
	   c12=c1>>1;
	   c22=c2>>1;

	   *sumNew = ((*sumNew) + c12)/c1;
	   *sumOld = ((*sumOld) + c22)/c2;

	   *wNew = (infoC[i].mw+c12)/c1;
	   *wOld = (infoC[j].mw+c22)/c2;

	   *hNew = (infoC[i].mh+c12)/c1;
	   *hOld = (infoC[j].mh+c22)/c2;

	   if( *wNew== *wOld && *hNew== *hOld &&
		   *sumNew== *sumOld )
	   return TRUE;

	   return FALSE;
}
///////////////////
extern SINT DistanceHausDLL(BYTE  *b1,SINT xbyte1,SINT yrow1,
						BYTE  *b2,SINT xbyte2,SINT yrow2,
						SINT porog);
/*
static int TestRaster(int num,int numSymbol,Nraster_header *rh,SINT *nClus,
					  int i1,int i2)
{
 int sum1=0;
 int sum2=0;

	for(i=0;i<numSymbol;i++)
	{
		if(nClus[i] == i1)
		{
			sum1+=DistanceHausDLL(rh[num].pHau,rh[num].byte1,SINT yrow1,
						BYTE  *b2,SINT xbyte2,SINT yrow2,
						SINT porog);
		}

	}
	return 0;
}
*/
////////////////////////
static int TrySubdivide(int numSymbol,Nraster_header *rh,
			    SINT *nClus,InfoCluster *infoC,
				int  nCluster,int numNew,
				Word32 *testField,Word32 *addField,
				int *smes,int *uniqal )
{
   int i,let;
   Word32 field[NFIELDDWORD];
   int nNul;
   int oldFir;
   InfoCluster saveInfo;
   int clus;
   int sumNew,sumOld,wNew,wOld,hNew,hOld;

   for(let='0';let<256;let++)
   {
	   // ���� ��� ����� ?
	   if( nCluster + numNew >= MAXWEICLUS )
		    break;

	   if( (clus=uniqal[let]) > 0 )
		 continue;

	   if( (clus=smes[let]) <= 0 ) continue;
	   if( clus > nCluster )
		   continue;

	   saveInfo=infoC[clus-1];

	   oldFir = infoC[clus-1].first;
       memset(infoC+clus-1,0,sizeof(InfoCluster));
	   infoC[clus-1].let=saveInfo.let;
	   infoC[nCluster+numNew]=infoC[clus-1];

	   for(i=oldFir,sumNew=sumOld=0,nNul=0;i<numSymbol;i++)
	   {
          if(nClus[i]!=clus)
			  continue;

		  if(rh[i].nField <= 0 || rh[i].nField > MAXFIELD )
		  {
			  nNul++;
			  nClus[i]=2*(nCluster+numNew+1);

			  continue;
		  }

		  MakeDWORDField(rh[i].nField,field);


		  if( IntersectFields(field,testField) )
		  {

			  FillInfo(infoC+nCluster+numNew,rh+i,i);

			  sumNew+=GetVes(rh+i);
			  nClus[i]=nCluster+numNew+1;

		  }
		  else
		  {
		   sumOld+=GetVes(rh+i);
		   FillInfo(infoC+clus-1,rh+i,i);

		  }
	   }

	   if( infoC[clus-1].count <= 0 ||
		   infoC[nCluster+numNew].count <= 0 ||
           BadCluster(infoC+clus-1) ||
		   BadCluster(infoC+nCluster+numNew ) ||
		   SameSizes(infoC,nCluster+numNew,clus-1,
			 &sumNew,&wNew,&hNew,&sumOld,&wOld,&hOld)
		 )
	   { // restore old
          infoC[clus-1]=saveInfo;
		  for(i=oldFir;i<numSymbol;i++)
		  {
			  if( nClus[i]== nCluster+numNew+1 ||
				  nClus[i]== 2*(nCluster+numNew+1)
				  )
				  nClus[i]=clus;
		  }
          continue;
	   }

	   if( nNul > 0 )
	   {
         int k,m;
		 for(i=oldFir;i<numSymbol;i++)
		  {
			 if(nClus[i]!=2*(nCluster+numNew+1))
			     continue;
			 k=GetVes(rh+i);

			 if( (m=abs(rh[i].w-wNew) -abs(rh[i].w-wOld)) < 0 ||
			      m == 0 && abs(k-sumOld) < abs(k-sumNew) )
			     m=nCluster+numNew+1;
			 else
				 m=clus;
			 nClus[i]=m;
			 infoC[m-1].count++;
			 infoC[m-1].mw+=rh[i].w;
			 infoC[m-1].mh+=rh[i].h;
			 if(i<infoC[m-1].first)
				 infoC[m-1].first=i;
		  }
       }

	   { // middle value
	   int m=nCluster+numNew;
	   infoC[m].mw = (infoC[m].mw+(infoC[m].count>>1))/infoC[m].count;
	   infoC[m].mh = (infoC[m].mh+(infoC[m].count>>1))/infoC[m].count;
	   infoC[m].good = 1; // mark
       infoC[clus-1].mw = (infoC[clus-1].mw+(infoC[clus-1].count>>1))/infoC[clus-1].count;
	   infoC[clus-1].mh = (infoC[clus-1].mh+(infoC[clus-1].count>>1))/infoC[clus-1].count;
	   infoC[clus-1].good = 1; // mark
	   }


	   numNew++;
   }

   return numNew;
}
///////////////

int TryDivide(int numSymbol,Nraster_header *rh,SINT *nClus,
			  int numCluster )
{
  int i,j;
  Word32 testField[NFIELDDWORD];
  Word32 addField[NFIELDDWORD],sameField[NFIELDDWORD];
  int numNew=0;
  int numSmes;

  InfoCluster *infoC =infoClusterStat;
  int         *countC= countCluster;
  Word32 dividedFields[NFIELDDWORD]={0,0};

  if(numCluster >= MAXWEICLUS )
	  return numCluster;

  GetClusterStatistic( numSymbol, numCluster, rh,
					nClus,infoC,countC,NULL,NULL,FALSE );

  for(j=0;j<numCluster;j++)
  {
    if(numCluster +numNew >= MAXWEICLUS)
		break;
	if( infoC[j].good )  // tested
		continue;

	if( BadCluster(infoC+j) ) continue;

	i=infoC[j].let;
	if(i<0 || i>=256) continue;
	if( countC[i] <= 1 )
	  continue;

	// ������� ������ �����-�����
	if( !LetDigSymbol(i) )
		continue;

	if( infoC[j].count < POROG_TEST)
		continue;
	if( EmptyField(infoC[j].fields) )
		continue;

	SetFields( testField,infoC[j].fields);
	if( IntersectFields(testField, dividedFields) )
		continue;

	if( GetAddField(j,testField,i,numCluster+numNew,infoC,addField)==0 ||
		EmptyField(addField) )
	     continue;

	memset(smes,0,256*sizeof(int));
	memset(uniqal,0,256*sizeof(int));
	numSmes=0;
	if( GetSameField(testField,addField,i,
		numCluster+numNew,infoC,&numSmes,sameField)==0 ||
		EmptyField(sameField) )
		continue;
	if( numSmes <= 0 )
		continue;

	AddFields(dividedFields, sameField);

    numNew=TrySubdivide(numSymbol,rh,nClus,
		infoC,numCluster,numNew, sameField,addField,
		smes,uniqal);
  }

  return numCluster+numNew; // ��������� �����
}
///////////////////////

