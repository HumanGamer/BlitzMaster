#pragma once


#define DIRCHANGE_FILE_NAME     (1<<0)
#define DIRCHANGE_DIR_NAME      (1<<1)
#define DIRCHANGE_LAST_WRITE    (1<<2)

class DirWatcherImp;


class DirWatcher
{
   public:

      DirWatcher();
      virtual ~DirWatcher();

   public:

      bool SetWatch( const wxString& dir, int flags, const wxArrayString& exclude );
      void Clear();

      int GetSignaled( wxArrayString* signaled );

   protected:

      DirWatcherImp* m_Impl;
};


class DirWatcherImp
{
   public:
      virtual ~DirWatcherImp() {};
      virtual bool SetWatch( const wxString& dir, int flags, const wxArrayString& exclude ) = 0;
      virtual int GetSignaled( wxArrayString* signaled ) = 0;
      virtual void Clear() = 0;
};
