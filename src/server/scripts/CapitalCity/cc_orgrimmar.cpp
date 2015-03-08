#include "SpellMgr.h"
#include "cc_orgrimmar.h"

class Orgrimmar_CapitalCityScript : public CapitalCityScript
{
public:
    Orgrimmar_CapitalCityScript() : CapitalCityScript("cc_orgrimmar") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Orgrimmar();
    }
};

void AddSC_cc_orgrimmar()
{
    new Orgrimmar_CapitalCityScript();
}