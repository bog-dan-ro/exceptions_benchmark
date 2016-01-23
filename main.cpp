#include <chrono>
#include <exception>
#include <iostream>

using namespace std;

static uint32_t checkPoint = 100;
static const uint32_t testCount = 1000000000l;

uint64_t toInt(uint64_t value, bool *ok) noexcept
{
    bool valid = true;
    if (value % checkPoint == 0)
        valid = false;

    if (ok)
        *ok = valid;

    if (!valid)
        return value;

    return ++value;
}

uint64_t toInt(uint64_t value)
{
    if (value % checkPoint == 0)
        throw std::invalid_argument("bla bla");
    return ++value;
}

int main()
{
    cout << "Benchmarking exceptions, doing " << testCount << " function calls" << endl;
    while (checkPoint < testCount) {
        cout << "Throw an error every " << checkPoint << " calls" << endl;
        auto startError = chrono::high_resolution_clock::now();
        for (uint64_t test = 0; test < testCount;) {
            bool ok;
            test = toInt(test, &ok);
            if (!ok)
                ++test;
        }
        auto stopError = chrono::high_resolution_clock::now();

        auto startThrow = chrono::high_resolution_clock::now();
        for (uint64_t test = 0; test < testCount;) {
            try {
                test = toInt(test);
            } catch (...) {
                ++test;
            }
        }
        auto stopThrow = chrono::high_resolution_clock::now();
        auto errorTicks = (stopError - startError).count();
        auto throwTicks = (stopThrow - startThrow).count();

        cout << "Error ticks " << errorTicks << endl << "Throw ticks " << throwTicks << endl;
        if (errorTicks > throwTicks) {
            auto ratio = double(errorTicks)/double(throwTicks) ;
            cout << "Throw is x" <<  ratio << " times (" << (ratio -1) * 100 << "%) faster" << endl;
        } else {
            auto ratio = double(throwTicks)/double(errorTicks);
            cout << "Error is x" <<  ratio << " times (" << (ratio -1) * 100 << "%) faster" << endl;
        }
        cout << "-------------------------------" << endl;
        checkPoint *= 10;
    }
    return 0;
}
