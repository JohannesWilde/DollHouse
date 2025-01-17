#include <cassert>
#include <iostream>

struct TestDatum
{
    uint8_t value0;
    uint8_t value1;
};

static TestDatum testData[3] = {
    {0, 1},
    {2, 3},
    {4, 5},
};

static size_t determineIndex(TestDatum const & datum)
{
    return (&datum - testData);
}

// -----

template<size_t (*getIndex_)(void const *)>
class IndexBasedClass
{
public:
    IndexBasedClass(uint8_t const value)
        : value_(value)
    {
        // intentionally empty
    }

    size_t getIndex() const
    {
        return getIndex_(this);
    }

    uint8_t getValue() const
    {
        return value_;
    }

private:
    uint8_t const value_;
};

size_t getMyIndexBaseClassIndex(void const *);

typedef IndexBasedClass<getMyIndexBaseClassIndex> MyIndexBaseClass;

static MyIndexBaseClass myInstances[3] = {
    {0},
    {2},
    {1},
};

size_t getMyIndexBaseClassIndex(void const * const instance)
{
    return static_cast<MyIndexBaseClass const *>(instance) - myInstances;
}

// -----

template<size_t (*getIndex_)(void const *)>
class IndexBasedClassEmpty
{
public:
    IndexBasedClassEmpty()
    {
        // intentionally empty
    }

    size_t getIndex() const
    {
        return getIndex_(this);
    }
};

size_t getMyIndexBaseClassEmptyIndex(void const *);

typedef IndexBasedClassEmpty<getMyIndexBaseClassEmptyIndex> MyIndexBaseClassEmpty;

static MyIndexBaseClassEmpty myEmptyInstances[3] = {
    {},
    {},
    {},
};

size_t getMyIndexBaseClassEmptyIndex(void const * const instance)
{
    return static_cast<MyIndexBaseClassEmpty const *>(instance) - myEmptyInstances;
}

// -----

int main(int argc, char* argv[])
{
    for (TestDatum const & datum : testData)
    {
        size_t const index = determineIndex(datum);
        std::cout << "index: " << index << std::endl;
    }

    for (MyIndexBaseClass const & myInstance : myInstances)
    {
        size_t const index = myInstance.getIndex();
        uint8_t const value = myInstance.getValue();
        std::cout << index << ": " << static_cast<unsigned>(value) << std::endl;
    }

    for (MyIndexBaseClassEmpty const & myEmptyInstance : myEmptyInstances)
    {
        size_t const index = myEmptyInstance.getIndex();
        std::cout << "index: " << index << std::endl;
    }

    std::cout << "sizeof(MyIndexBaseClassEmpty): " << sizeof(MyIndexBaseClassEmpty) << std::endl;
    std::cout << "sizeof(MyIndexBaseClass): " << sizeof(MyIndexBaseClass) << std::endl;

    std::cout << "testImplicitIndexing succeeded." << std::endl;
}
