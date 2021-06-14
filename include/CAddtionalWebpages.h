#pragma once


class CAddtionalWebpages 
{
//Stingleton START
public:
    static CAddtionalWebpages &getInstance()
    {
        static CAddtionalWebpages S;
        return S;
    }

    CAddtionalWebpages(const CAddtionalWebpages &) = delete;
    void operator=(CAddtionalWebpages const &) = delete;

private:
    CAddtionalWebpages();
    ~CAddtionalWebpages();
//Stingleton END
public:
    void setupAdditionalWebPageNormalMode();
    void setupAdditionalWebpagesAPMode();
};