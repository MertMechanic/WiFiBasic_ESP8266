#pragma once


/**
 * @brief Customize this for implementing your own webpagesroutes
 *        inherit  from this class and override the virutal methods
 */
class CAddtionalWebpages 
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
    CAddtionalWebpages();
    ~CAddtionalWebpages();
//Stingleton END
public:
    virtual void setupAdditionalWebPageNormalMode();
    virtual void setupAdditionalWebpagesAPMode();
};