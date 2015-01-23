#include <iostream>
#include <string>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
class Button
{
    typedef boost::signals2::signal<void (int x, int y)> OnClick;
public:
    void addOnClick(const OnClick::slot_type& slot);
    void press(int x, int y)
    {
        onClick(x, y);
    }
private:
        OnClick onClick;
};

void Button::addOnClick(const OnClick::slot_type& slot)
{
   onClick.connect(slot);
}


void printCoordinates(long x, long y)  
{
    std::cout<<"Button Clicked @(" << x << "," << y <<")\n";  
}

void button_click_test()
{
    Button button;
    button.addOnClick(&printCoordinates);
    std::cout<<"===== button onclick test\n";
    button.press(200,300);
    button.press(20,30);
    button.press(19,3);
}

int main()
{
    button_click_test();

    return 0;
}
