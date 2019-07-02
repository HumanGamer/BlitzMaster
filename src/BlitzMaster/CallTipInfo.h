#pragma once


class CallTipInfo
{
   public:

      CallTipInfo( int tabSize = 3 );
      ~CallTipInfo();

      void SetTip( const wxString& decl, const wxString& desc );

      const wxString& GetTip() const { return m_FormattedTip; }

      bool IsEmpty() const { return m_FormattedTip.IsEmpty(); }

      operator const wxString&() const { return m_FormattedTip; }

      bool GetArgumentRange( int param, int* start, int* end ) const;

      void RemoveThis();

      static wxString FormatTip( const wxString& tip, int tabSize );

   protected:

      const int      m_TabSize;

      wxString       m_FormattedTip;
      wxString       m_Tip;
      wxString       m_Description;

      struct Arg
      {
         int start;
         int end;
      };

      WX_DEFINE_ARRAY( Arg*, ArgArray );
      ArgArray m_Args;
};

WX_DEFINE_ARRAY( CallTipInfo*, CallTipInfoArray );
