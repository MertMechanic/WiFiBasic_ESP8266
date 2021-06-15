
#pragma once

#include "CAddtionalWebpages.h"

/**
 * @brief Example for Custommizing your own webpages
 * 
 */
class CCustomAddionalWebpages : public CAddtionalWebpages
{
    public:
    void setupAdditionalWebPageNormalMode() override;
    void setupAdditionalWebpagesAPMode() override;

};