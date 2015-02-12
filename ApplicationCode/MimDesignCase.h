#pragma once



#include "cafPdmObject.h"
#include "cafPdmField.h"


class MimDesignCase : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;
public:
    MimDesignCase(void);
    virtual ~MimDesignCase(void);

    // Fields
    caf::PdmField<QString> name;
    
    // 
    virtual caf::PdmFieldHandle* userDescriptionField();

    // 
    void updateModelAndRedraw();
    
private:
};
