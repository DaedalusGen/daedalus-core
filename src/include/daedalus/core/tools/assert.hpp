#ifndef __DAEDALUS_ASSERT__
#define __DAEDALUS_ASSERT__

#define DAE_ASSERT_TRUE(expression, error) \
if(!(expression)) { \
	throw error; \
}

#define DAE_ASSERT_TRY(body, error) \
try body catch(const std::exception& e) { \
	throw error; \
}

#ifdef DEBUG

#define DAE_DEBUG(body) \
body

#define DAE_DEBUG_ASSERT_TRUE(name, expression, error) \
std::cout << "Asserting " << name << "..." << std::endl; \
DAE_ASSERT_TRUE(expression, error) \
std::cout << "Asserted " << name << " successfully" << std::endl;

#define DAE_DEBUG_LOG(str) \
std::cout << "[LOG] " << str << "\n\n";

#define DAE_DEBUG_WARN(str) \
std::cout << "[WARN] " << str << "\n\n";

#define DAE_DEBUG_ERROR(str) \
std::cout << "[ERR] " << str << "\n\n";

#else

#define DAE_DEBUG(body)

#define DAE_DEBUG_ASSERT_TRUE(name, expression, error)

#define DAE_DEBUG_LOG(str)

#define DAE_DEBUG_WARN(str)

#define DAE_DEBUG_ERROR(str)

#endif

#endif
