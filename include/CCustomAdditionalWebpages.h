
#pragma once

#include "CAdditionalWebpages.h"

/**
 * @brief Example for Custommizing your own webpages
 * 
 */
class CCustomAdditionalWebpages : public CAdditionalWebpages
{
    public:
    void setupAdditionalWebPageNormalMode() override;
    void setupAdditionalWebpagesAPMode() override;

};