#include "SpellMgr.h"
#include "cc_stormwind.h"

class Stormwind_CapitalCityScript : public CapitalCityScript
{
public:
    Stormwind_CapitalCityScript() : CapitalCityScript("cc_stormwind") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Stormwind();
    }
};

void AddSC_cc_stormwind()
{
    new Stormwind_CapitalCityScript();
}