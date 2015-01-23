#include <iostream>
#include <string>
#include <boost/signals2.hpp>

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

class CTarget : public boost::signals2::trackable
{
public:
    virtual ~CTarget()
    {
        std::cout << __FUNCTION__ << std::endl;
        m_con.disconnect();
    }
    CTarget()
    {
        m_strMsg = "Hello, world";
    }
    void RegisterSolts(CSource& source)
    {
        m_con = source.Connect(boost::bind(&CTarget::SomeHandle, this));
        m_con = source.Connect(boost::bind(&CTarget::SomeHandle1, this));
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
    boost::signals2::connection m_con;
};

int main()
{
    CSource source;
    {
        CTarget target;
        
        target.RegisterSolts(source);
        source.OnSig();
    }
    source.OnSig();

    return 0;
}

