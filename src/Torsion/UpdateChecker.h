#pragma once


class UpdateChecker
{
public:

   UpdateChecker();
   virtual ~UpdateChecker();

   bool CheckAvailable( wxString* version );

};
