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

#ifndef __POLYBLOCK_H__
#define __POLYBLOCK_H__
#pragma pack (push,8)

#include "table.h"

# define  TYPE_TEXT			CPAGE_GetInternalType("TYPE_TEXT")
# define  TYPE_IMAGE		CPAGE_GetInternalType("TYPE_IMAGE")
# define  TYPE_TABLE		CPAGE_GetInternalType("TYPE_TABLE")
# define  TYPE_EMPTY		CPAGE_GetInternalType("TYPE_EMPTY")
# define  TYPE_DESC			CPAGE_GetInternalType("TYPE_DESC")
# define  TYPE_DESK			TYPE_DESC
# define  TYPE_PICTURE      CPAGE_GetInternalType("TYPE_PICTURE")
# define  TYPE_SCROLL       CPAGE_GetInternalType("TYPE_SCROLL")
# define  TYPE_LINE         CPAGE_GetInternalType("TYPE_LINE")
# define  MaxNum			1000

//��������� ���������� ���������� (� ���������� ��������� � � ������ �������) ����: ������� - ������� ��� ������������
//01.01.01 �������
# define TYPE_NEGATIVE 1
# define TYPE_POSITIVE 0

//��������� ���������� ����������� ���������� (� ���������� ��������� � � ������ �������)
//01.01.01 �������
# define TYPE_LEFTRIGHT 0
# define TYPE_UPDOWN    1
# define TYPE_DOWNUP    2


//�������� ������������� �������� ������ ������. ��� ������������ ��� ���������� ��������
# define VISIBLE_LINE      0
# define HIDE_LINE         1
# define GRAYED_LINE       2
# define DOTTED_LINE       4
# define DASHED_LINE       8
# define DOUBLED_LINE      16
# define NORMAL_LINE       0	// ���������



typedef struct tagStructBool16
{
//////////////////////////////////////////
#define    BIT_0                      0x01
#define    BIT_1                      0x02
#define    BIT_2                      0x04
#define    BIT_3                      0x08
#define    BIT_4                      0x10
#define    BIT_5                      0x20
#define    BIT_6                      0x40
#define    BIT_7                      0x80
//////////////////////////////////////////
	char Type;
	char Visible;
} StructBool16;

typedef struct tagVertex
{
	Int32 x;
	Int32 y;
	Int32 mark;
} VERTEX;

typedef struct tagCommon
{
	Handle type;//�����, ��������, �������;
	Int16 number;//���������� �����
	Int16 Color;
	Bool Select;//
	Int16 count;
	Point32 Vertex[MaxNum];
// Almi 18.04.00
#define POS_NEGTABCAP   0x00000001 // ��������� ���������� ��������� �������
#define POS_NEGTXTCAP   0x00000002 // ��������� ���������� ��������� ������
#define POS_MATCH       0x00000004 // ������������ � ��������� ��������� ������������
	Word32 Flags;
} COMMON;

typedef struct tagPOLY
{
	COMMON com;

	Int32 mark[MaxNum];

	Int32 alphabet;//�����,����� � �����, �����
	Bool16 negative;//������� = TYPE_NEGATIVE, ������� = TYPE_POSITIVE;//     01.01.01 �������
	Int16 orient;//TYPE_NORD- ������ ���� (����������), TYPE_WEST- ����� �� ����� ����, TYPE_OST- ����� �� ������ ����.
} POLY_;

typedef struct tagTABLE
{
	COMMON com;

	Int32 num_colons;//����� �������
	Int32 num_rows;//����� �����
	Int32 LineY[MaxHorLines/*MaxHorLines*/-1];//���������� ����� (������� ��������� � ������ ������, ������� ������ ������� �� ���������)
	Int32 LineX[MaxVerLines-1];//���������� ������� (������� ��������� � ������ ����� ��������)
	Bool16 Visible[MaxHorLines][MaxVerLines][2];//������-�������� ���� �����: ��������, ����������, �����
	//StructBool16 Visible[MaxHorLines][MaxVerLines][2];//������-�������� ���� �����: ��������, ����������, �����
	Bool16 TypeCell[MaxHorLines][MaxVerLines];//��� ������
	Int32 Skew;
	Bool16 Negative[MaxHorLines][MaxVerLines];//������� = 1, ������� = 0;//     01.01.01 �������
	Int16 Orient[MaxHorLines][MaxVerLines];//TYPE_UPDOWN- ������ ���� � �.�. �� ��������� ����

	char    iSet; //����� ����, ��������
	char    TypeTablDetail;
	char    Active; // 0 - ���������, 1 - �������� � ����
	char    reserv[3];
	//Int16 SetNum;//��������� � ����
	//Bool16 bActive;//��� ���������� �������� ������� � ����
	//Int16 GlobNum;//���������� ���������

} TABLE_;
/*
typedef struct tagLINE
{
	COMMON com;

	Int32 width;//������

	Int32 visi;//������� ���: �������,���������,�������,�������.

} LINE_;
*/
#pragma pack (pop)
#endif
