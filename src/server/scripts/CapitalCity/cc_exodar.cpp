#include "SpellMgr.h"
#include "cc_exodar.h"

class Exodar_CapitalCityScript : public CapitalCityScript
{
public:
    Exodar_CapitalCityScript() : CapitalCityScript("cc_exodar") { }

    CapitalCity* GetCapitalCity() const override
    {
        return new CapitalCity_Exodar();
    }
};

void AddSC_cc_exodar()
{
    new Exodar_CapitalCityScript();
}