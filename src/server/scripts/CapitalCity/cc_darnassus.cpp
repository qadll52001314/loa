#include "SpellMgr.h"
#include "cc_darnassus.h"

class Darnassus_CapitalCityScript : public CapitalCityScript
{
public:
    Darnassus_CapitalCityScript() : CapitalCityScript("cc_darnassus") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Darnassus();
    }
};

void AddSC_cc_darnassus()
{
    new Darnassus_CapitalCityScript();
}