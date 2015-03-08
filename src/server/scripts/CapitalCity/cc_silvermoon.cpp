#include "SpellMgr.h"
#include "cc_silvermoon.h"

class Silvermoon_CapitalCityScript : public CapitalCityScript
{
public:
    Silvermoon_CapitalCityScript() : CapitalCityScript("cc_silvermoon") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Silvermoon();
    }
};

void AddSC_cc_silvermoon()
{
    new Silvermoon_CapitalCityScript();
}