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

///////////////////////////////////////////////////////////////////////////////
//
//  Module: Utility.cpp
//
//    Desc: See Utility.h
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utility.h"
#include "atldlgs.h"
#include "resource.h"


FILETIME CUtility::getLastWriteFileTime(CString sFile)
{
   FILETIME          ftLocal = {0};
   HANDLE            hFind;
   WIN32_FIND_DATA   ff32;
   hFind = FindFirstFile(sFile, &ff32);
   if (INVALID_HANDLE_VALUE != hFind)
   {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftLocal);
      FindClose(hFind);
   }
   return ftLocal;
}


CString CUtility::getAppName()
{
   TCHAR szFileName[_MAX_PATH];
   GetModuleFileName(NULL, szFileName, _MAX_FNAME);

   CString sAppName; // Extract from last '\' to '.'
   sAppName = szFileName;
   sAppName = sAppName.Mid(sAppName.ReverseFind(_T('\\')) + 1)
                      .SpanExcluding(_T("."));

   return sAppName;
}


CString CUtility::getTempFileName()
{
   TCHAR szTempDir[MAX_PATH - 14]   = _T("");
   TCHAR szTempFile[MAX_PATH]       = _T("");

   if (GetTempPath(MAX_PATH - 14, szTempDir))
      GetTempFileName(szTempDir, getAppName(), 0, szTempFile);

   return szTempFile;
}


CString CUtility::getSaveFileName()
{
   CString sFilter((LPCTSTR)IDS_ZIP_FILTER);
//   sFilter.Replace('|','\0'); - �� �������� ��-�� ������ � WTL
   int len = sFilter.GetLength();
   char * strPtr = sFilter.GetBuffer(len);
   for(int i = 0; i < len; i++)
	   if (strPtr[i] == '|')
		   strPtr[i] = 0;
   sFilter.ReleaseBuffer(len);


   CFileDialog fd(
      FALSE,
      _T("zip"),
      formatSaveFileName(),
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      sFilter);

	if (IDOK == fd.DoModal())
   {
      DeleteFile(fd.m_szFileName);  // Just in-case it already exist
      return fd.m_szFileName;
   }

   return _T("");
}
CString CUtility::formatSaveFileName()
{
   //����������� ������� ����
   time_t ltime;
   time( &ltime );
   char dateBuf[100] = "";
   strftime(dateBuf, 100, "%Y%m%d %H-%M-%S", localtime(&ltime));
   CString sFile;
   sFile.Format(_T("%s%s"), CUtility::getAppName(), dateBuf);
   return sFile;
}
