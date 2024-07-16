#ifndef __DAEDALUS_UNIT__
#define __DAEDALUS_UNIT__

#include <iostream>
#include <stdexcept>

#define DAE_ASSERT_TRUE(expression, error) \
if(!(expression)) { \
	throw error; \
}

#define DAE_ASSERT_TRY(body, error) \
try body catch(const std::exception& e) { \
	throw error; \
}

#ifdef DEBUG

#define DAE_DEBUG_ASSERT_TRUE(name, expression, error) \
std::cout << "Asserting " << name << "..." << std::endl; \
DAE_ASSERT_TRUE(expression, error) \
std::cout << "Asserted " << name << " successfully" << std::endl;

#endif

#endif