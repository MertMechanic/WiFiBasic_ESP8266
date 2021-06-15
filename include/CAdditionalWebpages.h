#pragma once


/**
 * @brief Customize this for implementing your own webpagesroutes
 *        inherit  from this class and override the virutal methods
 */
class CAdditionalWebpages 
{
//Stingleton START
public:
//     static CAddtionalWebpages &getInstance()
//     {
//         static CAddtionalWebpages S;
//         return S;
//     }

//     CAddtionalWebpages(const CAddtionalWebpages &) = delete;
//     void operator=(CAddtionalWebpages const &) = delete;

// private:
    CAdditionalWebpages();
    ~CAdditionalWebpages();
//Stingleton END
public:
    virtual void setupAdditionalWebPageNormalMode();
    virtual void setupAdditionalWebpagesAPMode();
};