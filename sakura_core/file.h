/*
	Copyright (C) 2002, SUI
	Copyright (C) 2008, kobake

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

		1. The origin of this software must not be misrepresented;
		   you must not claim that you wrote the original software.
		   If you use this software in a product, an acknowledgment
		   in the product documentation would be appreciated but is
		   not required.

		2. Altered source versions must be plainly marked as such,
		   and must not be misrepresented as being the original software.

		3. This notice may not be removed or altered from any source
		   distribution.
*/
#ifndef SAKURA_FILE_H_
#define SAKURA_FILE_H_

#include <stdio.h>
#include <tchar.h>

bool fexist(LPCTSTR pszPath); //!< �t�@�C���܂��̓f�B���N�g�������݂����true

bool IsFilePath( const char*, int*, int*, bool = true );
bool IsFileExists(const TCHAR* path, bool bFileOnly = false);


// 2005.11.26 aroka
bool IsLocalDrive( const TCHAR* pszDrive );

//���T�N���ˑ�
FILE *_tfopen_absexe(LPCTSTR fname, LPCTSTR mode); // 2003.06.23 Moca
FILE *_tfopen_absini(LPCTSTR fname, LPCTSTR mode, BOOL bOrExedir = TRUE); // 2007.05.19 ryoji

//�p�X�����񏈗�
void CutLastYenFromDirectoryPath( TCHAR* );						/* �t�H���_�̍Ōオ���p����'\\'�̏ꍇ�́A��菜�� "c:\\"���̃��[�g�͎�菜���Ȃ�*/
void AddLastYenFromDirectoryPath( TCHAR* );						/* �t�H���_�̍Ōオ���p����'\\'�łȂ��ꍇ�́A�t������ */
void SplitPath_FolderAndFile( const char*, char*, char* );		/* �t�@�C���̃t���p�X���A�t�H���_�ƃt�@�C�����ɕ��� */
BOOL GetLongFileName( const TCHAR*, TCHAR* );					/* �����O�t�@�C�������擾���� */
BOOL CheckEXT( const TCHAR*, const TCHAR* );					/* �g���q�𒲂ׂ� */
bool _IS_REL_PATH(const TCHAR* path);											//!< ���΃p�X�����肷��B2003.06.23 Moca

//���T�N���ˑ�
void GetExedir( LPTSTR pDir, LPCTSTR szFile = NULL );
void GetInidir( LPTSTR pDir, LPCTSTR szFile = NULL ); // 2007.05.19 ryoji
void GetInidirOrExedir( LPTSTR pDir, LPCTSTR szFile = NULL, BOOL bRetExedirIfFileEmpty = FALSE ); // 2007.05.22 ryoji

//�t�@�C������
class CFileTime{
public:
	CFileTime(){ ClearFILETIME(); }
	CFileTime(const FILETIME& ftime){ SetFILETIME(ftime); }
	//�ݒ�
	void ClearFILETIME(){ m_ftime.dwLowDateTime = m_ftime.dwHighDateTime = 0; m_bModified = true; }
	void SetFILETIME(const FILETIME& ftime){ m_ftime = ftime; m_bModified = true; }
	//�擾
	const FILETIME& GetFILETIME() const{ return m_ftime; }
	const SYSTEMTIME& GetSYSTEMTIME() const
	{
		//�L���b�V���X�V -> m_systime, m_bModified
		if(m_bModified){
			m_bModified = false;
			FILETIME ftimeLocal;
			if(!::FileTimeToLocalFileTime( &m_ftime, &ftimeLocal ) || !::FileTimeToSystemTime( &ftimeLocal, &m_systime )){
				memset(&m_systime,0,sizeof(m_systime)); //���s���[���N���A
			}
		}
		return m_systime;
	}
	const SYSTEMTIME* operator->() const{ return &GetSYSTEMTIME(); }
	//����
	bool IsZero() const
	{
		return m_ftime.dwLowDateTime == 0 && m_ftime.dwHighDateTime == 0;
	}
protected:
private:
	FILETIME m_ftime;
	//�L���b�V��
	mutable SYSTEMTIME	m_systime;
	mutable bool		m_bModified;
};
bool GetLastWriteTimestamp( const TCHAR* filename, CFileTime* pcFileTime ); //	Oct. 22, 2005 genta

//�����񕪊�
void my_splitpath ( const char *comln , char *drv,char *dir,char *fnm,char *ext );
char	*sjis_strrchr2( const char *pt , const char ch1 , const char ch2 );

#endif /* SAKURA_FILE_H_ */
/*[EOF]*/