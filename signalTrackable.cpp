#include <iostream>
#include <string>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

class NewsItem
{
public:
    NewsItem();

    NewsItem(const NewsItem& item)
    {
        msg = item.msg;
    }

    NewsItem(const std::string message)
    {
        msg = message;
    }

    const std::string text() const
    {
        return msg;
    }

private:
    std::string msg;
};

class MessageArea
{
public:
    MessageArea(){messageText = "default";}

    MessageArea(const MessageArea& messageArea)
    {
        messageText = messageArea.messageText;
    }

    MessageArea(const std::string  message)
    {
        messageText = message;
    }

    void update()
    {
        std::cout <<"messageText="<<messageText<<std::endl;
    }

    std::string messageText;
};

class NewsMessageArea : public MessageArea, public boost::signals2::trackable//trackable is important
{
public:
    NewsMessageArea(const std::string message):MessageArea(message){}
    void displayNews(const NewsItem& news)
    {
        messageText = news.text();
        update();
    }
};

int main()
{
    boost::signals2::signal<void (const NewsItem&) > deliverNews;
    NewsMessageArea* newsMessageArea = new NewsMessageArea("Hello, World!");
    deliverNews.connect(boost::bind(&NewsMessageArea::displayNews, newsMessageArea, _1));

    const NewsItem newItem("Hello, Kitty!");
    deliverNews(newItem);
   
    delete newsMessageArea;
    std::cout<<"*******Segmentation fault below if no trackable******"<<std::endl;
    deliverNews(newItem);

    return 0;
}
