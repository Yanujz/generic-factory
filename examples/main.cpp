#include <iostream>
#include "genericfactory.hpp"

// Base class
class Animal {
public:
    virtual void speak() = 0;

    virtual ~Animal() = default;
};

// Derived classes
class Dog : public Animal {
public:
    Dog()
    {
        std::cout << "Creating Dog" << std::endl;
    }

    virtual ~Dog()
    {
        std::cout << "Destroying Dog" << std::endl;
    }

    void speak() override
    {
        std::cout << "Woof!" << std::endl;
    }
};

class Cat : public Animal {
public:
    Cat()
    {
        std::cout << "Creating Cat" << std::endl;
    }

    virtual ~Cat()
    {
        std::cout << "Destroying Cat" << std::endl;
    }

    void speak() override
    {
        std::cout << "Meow!" << std::endl;
    }
};

int main(int argc, char** argv)
{
    GenericFactory<Animal, std::string> factory({
        { "dog", GenericFactory<Animal>::creator<Dog> },
        { "cat", GenericFactory<Animal>::creator<Cat> },
    });

    auto cat = factory.get("cat");


// Comment here
    {
        auto dog = factory.get("dog");
        if(dog)
        {
            dog->speak(); // Output: Woof!
        }
    }

    // End of scope for `dog`, it will be destroyed here.
    if(cat)
    {
        cat->speak();  // Output: Meow!
    }

    std::cout << "[MAIN] Destroying dog" << std::endl;
    factory.stop("dog");

    std::cout << "[MAIN] Getting a dog" << std::endl;
    auto dog = factory.get("dog");
    if(dog)
    {
        dog->speak();  // Output: Woof!
    }

    return 0;
}
