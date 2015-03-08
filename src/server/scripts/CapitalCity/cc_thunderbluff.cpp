#include "SpellMgr.h"
#include "cc_thunderbluff.h"

class Thunderbluff_CapitalCityScript : public CapitalCityScript
{
public:
    Thunderbluff_CapitalCityScript() : CapitalCityScript("cc_thunderbluff") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Thunderbluff();
    }
};

void AddSC_cc_thunderbluff()
{
    new Thunderbluff_CapitalCityScript();
}