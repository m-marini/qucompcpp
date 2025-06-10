#ifndef _testUnit_h_
#define _testUnit_h_

#include <string>

#define TEST(message)                                                                     \
    {                                                                                     \
        cout << "\033[1;32m" << __FILE__ << ":" << __LINE__ << ": " << (message) << endl; \
    }

#define ASSERT_TRUE(test)                                                                       \
    if (!(test))                                                                                \
    {                                                                                           \
        cerr << "\033[1;31m" << __FILE__ << ":" << __LINE__ << ": ASSERT_TRUE failed " << endl; \
    }

#define ASSERT_FALSE(test)                                                                       \
    if ((test))                                                                                  \
    {                                                                                            \
        cerr << "\033[1;31m" << __FILE__ << ":" << __LINE__ << ": ASSERT_FALSE failed " << endl; \
    }

#define ASSERT_EQUALS(expected, value)                                                                                                                   \
    if (!((value) == (expected)))                                                                                                                        \
    {                                                                                                                                                    \
        cerr << "\033[1;31m" << __FILE__ << ":" << __LINE__ << ": ASSERT_EQUALS failed (" << (value) << ") not equal to (" << (expected) << ")" << endl; \
    }

#define LOG(message)                                                      \
    {                                                                     \
        cout << __FILE__ << ":" << __LINE__ << ": " << (message) << endl; \
    }

#endif
