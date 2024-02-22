#ifndef GRAPHLOOM_COMMON_MACROS_H_
#define GRAPHLOOM_COMMON_MACROS_H_

/**
 * This module defines general purpose 
 * graphloom macros.
*/


#define GL_STRINGIFY_HELPER(X) #X
// turns plain text into string
#define GL_STRINGIFY(X) GL_STRINGIFY_HELPER(X)

// concatenates two strings
#define GL_CONCATENATE(x, y) x##y

#endif