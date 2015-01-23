#include <string>
#include<iostream>
#include <boost/signals2.hpp>
class CTarget : public boost::signals2::trackable
{
public:
    virtual ~CTarget()
    {
        std::cout << __FUNCTION__ << std::endl;
    }
    CTarget()
    {
        m_strMsg = "Hello World!";
    }
    void SomeHandle()
    {
        std::cout << __FUNCTION__ << std::endl;
        std::cout << m_strMsg << std::endl;
    }
    void SomeHandle1()
    {
        std::cout << __FUNCTION__ << std::endl;
    }
private:
    std::string m_strMsg;
};

class CSource
{
public:
    boost::signals2::connection Connect(const boost::signals2::signal<void(void)>::slot_type &sub)
    {
        return m_sig.connect(sub);
    }
    void OnSig()
    {
        m_sig();
    }
private:
    boost::signals2::signal<void(void)> m_sig;
};
int main()
{
    CSource source;
    boost::signals2::connection conn;
    {
        CTarget target;
        /*conn = */source.Connect(boost::bind(&CTarget::SomeHandle, &target));
        source.Connect(boost::bind(&CTarget::SomeHandle1, &target));
        source.OnSig();
    }
    source.OnSig();
return 0;
}
