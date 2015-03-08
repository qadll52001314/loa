#include "SpellMgr.h"
#include "cc_ironforge.h"

class Ironforge_CapitalCityScript : public CapitalCityScript
{
public:
    Ironforge_CapitalCityScript() : CapitalCityScript("cc_ironforge") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Ironforge();
    }
};

void AddSC_cc_ironforge()
{
    new Ironforge_CapitalCityScript();
}