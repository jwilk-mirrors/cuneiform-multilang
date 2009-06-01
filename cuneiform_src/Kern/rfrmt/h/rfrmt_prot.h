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
// frmt_prot.h
//
// ============================================================================

#ifndef __frmt_prot_h__
#define __frmt_prot_h__

#include <stddef.h>
#include <stdio.h>

BOOL CreateInternalFileForFormatter(FILE *fpInternalFileName);
#ifdef __cplusplus
	extern "C" {
#endif

 BOOL FullRtf(FILE *fpFileNameIn, const char *FileNameOut, Handle* hEdTree);
#ifdef __cplusplus
	}
#endif


//////////////////////////////////////////////////////////////////////////////////
//
#ifdef __RFRMT_EXTERN__
	#define EXTERN
	#define VAL(a) = a
#else
	#define EXTERN extern
	#define VAL(a)
#endif


EXTERN Handle hDebugRoot					     VAL( NULL );
EXTERN Handle hDebugMy            VAL( NULL );
EXTERN Handle hDebugKegl          VAL( NULL );
EXTERN Handle hDebugFrame         VAL( NULL );
EXTERN Handle hDebugLineTransfer  VAL( NULL );
EXTERN Handle hDebugAlign         VAL( NULL );
EXTERN Handle hDebugProfile							VAL( NULL );
EXTERN Handle hDebugProfStart     VAL( NULL );
EXTERN Handle hDebugProfTable     VAL( NULL );
EXTERN Handle hDebugProfEnd       VAL( NULL );
EXTERN Handle hDebugProfEndTable  VAL( NULL );
EXTERN Handle hDebugProfCell      VAL( NULL );
EXTERN Handle hDebugProfTmp       VAL( NULL );
EXTERN Handle hDebugProfWrite     VAL( NULL );
EXTERN Handle hDebugProfWriteRow  VAL( NULL );
EXTERN Handle hDebugProfWriteCell VAL( NULL );
EXTERN Handle hDebugProfWriteChar VAL( NULL );

EXTERN Handle hTest														 VAL( NULL );
EXTERN Handle hTestDIBData			VAL( NULL );
EXTERN Handle hTestTurn						VAL( NULL );
EXTERN Handle hTestRotate				VAL( NULL );
EXTERN Handle hTestGetMaskDIB VAL( NULL );
EXTERN Handle hTestWriteED			VAL( NULL );
EXTERN Handle hTestEnd												VAL( NULL );
EXTERN Handle hTestWriteMetafile		VAL( NULL );
EXTERN Handle hTestDeleteImage					VAL( NULL );

#endif // __frmt_prot_h__
