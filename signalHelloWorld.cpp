#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <iostream>
using namespace std;
using namespace boost;
struct HelloWorld
{
  void operator()() const
  {
    std::cout << "Hello, World!" << std::endl;
  }
};

struct HelloKitty
{
  void operator()() const
  {
    std::cout << "Hello, Kitty!" << std::endl;
  }
};
//
struct GoodMorning
{
      void operator()() const
      {
          std::cout << "... and good morning!" << std::endl;
      }
};

class Hello
{
    public:
        void print()
        {
            std::cout << "Hello, World!" << std::endl;
        }
};

class World : public boost::signals2::trackable
{
    public:
        void print()
        {
            std::cout << "Hello, World!" << std::endl;
        }
};

void print_hello()
{
    std::cout<<"Hello, world"<<std::endl;
}

void print_sum(float x, float y)
{
      std::cout << "The sum is " << x+y << std::endl;
}

void print_product(float x, float y)
{
      std::cout << "The product is " << x*y << std::endl;
}

void print_difference(float x, float y)
{
      std::cout << "The difference is " << x-y << std::endl;
}

void print_quotient(float x, float y)
{
      std::cout << "The quotient is " << x/y << std::endl;
}

float product(float x, float y) { return x*y; }
float quotient(float x, float y) { return x/y; }
float sum(float x, float y) { return x+y; }
float difference(float x, float y) { return x-y; }

template<typename T>
struct maximum
{
  typedef T result_type;

  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    // If there are no slots to call, just return the
    // default-constructed value
    if (first == last)
      return T();

    T max_value = *first++;
    while (first != last) {
      if (max_value < *first)
        max_value = *first;
      ++first;
    }

    return max_value;
  }
};

template<typename Container>
struct aggregate_values
{
  typedef Container result_type;

  template<typename InputIterator>
  Container operator()(InputIterator first, InputIterator last) const
  {
    return Container(first, last);
  }
};


// Signal with no arguments and a void return value
int main()
{
    // Connect a HelloWorld slot
    HelloWorld hello;
    HelloKitty kitty;
    GoodMorning goodMorning;

    std::cout<<"****************sig1******************"<<std::endl;
    boost::signals2::signal<void ()> sig1;
    sig1.connect(1 ,hello);
    sig1.connect(0 ,kitty);
    sig1.connect(0 ,goodMorning);
    sig1();

    // Call all of the slots
    std::cout<<"****************sig2******************"<<std::endl;
    boost::signals2::signal<void (float, float) > sig2;
    sig2.connect(&print_product);
    sig2.connect(&print_quotient);
    sig2.connect(&print_sum);
    sig2.connect(&print_difference);
    sig2(5, 3);

    std::cout<<"****************sig3*******************"<<std::endl;
    boost::signals2::signal<float (float, float) > sig3;
    sig3.connect(product);
    sig3.connect(quotient);
    sig3.connect(sum);
    sig3.connect(difference);
    std::cout<< *sig3(5,3)<<std::endl;

    std::cout<<"****************sig4*******************"<<std::endl;
    boost::signals2::signal<float (float, float), maximum<float> > sig4;
    sig4.connect(product);
    sig4.connect(quotient);
    sig4.connect(sum);
    sig4.connect(difference);
    std::cout<< sig4(5,3)<<std::endl;

    std::cout<<"****************sig5*******************"<<std::endl;
    boost::signals2::signal<float (float, float), aggregate_values<std::vector<float> > > sig5;
    sig5.connect(product);
    sig5.connect(quotient);
    sig5.connect(sum);
    sig5.connect(difference);
    std::vector<float> results = sig5(5, 3);
    std::copy(results.begin(), results.end(), std::ostream_iterator<float>(cout, "\n"));

    std::cout<<"***************connection management connected**************"<<std::endl;
    boost::signals2::signal<void ()> sig6;
    boost::signals2::connection c = sig6.connect(HelloWorld());
    if (c.connected())
    {
        // c is still connected to the signal
        sig6(); // Prints "Hello, World!"
    }

    c.disconnect(); // Disconnect the HelloWorld object
    assert(!c.connected());
    sig6(); // Does nothing: there are no connected slots 

    std::cout<<"***************connection management block**************"<<std::endl;
    boost::signals2::signal<void ()> sig7;
    boost::signals2::connection c1 = sig7.connect(HelloWorld());
    sig7();
    {
        boost::signals2::shared_connection_block block(c1);
        sig7();
        std::cout<<"c1 is blocked"<<std::endl;
    }
    assert(!c1.blocked());
    sig7();

    std::cout<<"***************connection management scoped**************"<<std::endl;
    boost::signals2::signal<void ()> sig8;
    {
        boost::signals2::scoped_connection c2(sig8.connect(HelloWorld()));
        sig8();
        std::cout<<"c2 is blocked"<<std::endl;
    }
    sig8();

    std::cout<<"***************connection management connected**************"<<std::endl;
    boost::signals2::signal<void ()> sig9;
    sig9.connect(print_hello);
    sig9();
    std::cout<<"Disconnected sig9"<<std::endl;
    sig9.disconnect(print_hello);
    sig9();

    std::cout<<"***************connection management use shared_ptr**************"<<std::endl;
    boost::signals2::signal<void ()> sig10;
    {
        boost::shared_ptr<Hello> w(new Hello());
        sig10.connect(boost::bind(&Hello::print, w.get()));
    }

    std::cout <<"num of slots = "<< sig10.num_slots() << std::endl;
    sig10();

    std::cout<<"***************connection management use trackable**************"<<std::endl;
    boost::signals2::signal<void ()> sig11;
    {
        boost::shared_ptr<World> w(new World());
        sig11.connect(boost::bind(&World::print, w.get()));
    }

    std::cout <<"num of slots = "<< sig11.num_slots() << std::endl;
    sig11();


    return 0;
}
