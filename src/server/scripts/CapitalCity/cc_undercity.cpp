#include "SpellMgr.h"
#include "cc_undercity.h"

class Undercity_CapitalCityScript : public CapitalCityScript
{
public:
    Undercity_CapitalCityScript() : CapitalCityScript("cc_undercity") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Undercity();
    }
};

void AddSC_cc_undercity()
{
    new Undercity_CapitalCityScript();
}