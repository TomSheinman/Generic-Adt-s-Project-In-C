#ifndef DEFS_H_
#define DEFS_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum e_bool { false, true } bool;
/** memory_failure - allocation to object failed
    argument_failure - function received null pointer */
typedef enum e_status { success, failure, memory_failure, argument_failure } status;

typedef void * Element;

typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif /* DEFS_H_ */
