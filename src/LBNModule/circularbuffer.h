#ifndef circularbuffer_h
#define circularbuffer_h

#include <stdint.h>
#include "canCom.h"

/**
 * define the variable type to store in the buffer
 */
typedef canComMsg cbtype;

/**
 * buffers come in two variants
 */
enum cbmode { OVERWRITE_IF_FULL, IGNORE_IF_FULL };

/**
 * buffer data, do not modify in client!
 */
typedef struct {
  cbtype* data;
  enum cbmode mode;
  int8_t size;
  int8_t start;
  int8_t count;
} cbuffer;

/**
 * initialize a new buffer, in case NULL is returned,
 * buffer init failed
 * 
 * Parameter:
 *  The wanted buffer size (int8_t size)
 *  The mode of the buffer see enum cbmode for options (enum cbmode mode)
 * 
 * Return:
 *  The buffer if succesfull, NULL if failed
 */
cbuffer* cbInit(int8_t size, enum cbmode mode);
/**
 * free the buffer, a new buffer can be created
 * instead.
 *
 * cbFree returns NULL to allow for b = cbFree(b);
 * 
 * Parameter:
 *  Pointer to the affected buffer
 */
cbuffer* cbFree(cbuffer* buffer);

/**
 * check whether data van be read from the buffer
 * 
 * Parameter:
 *  Pointer to the affected buffer
 */
bool cbAvailable(cbuffer* buffer);
/**
 * peek the oldest value in the buffer, value
 * remains available for read.
 * 
 * Parameter:
 *  Pointer to the affected buffer
 * 
 * Return:
 *  a variable from the buffer
 */
cbtype cbPeek(cbuffer* buffer);
/**
 * read and remove the oldest value in the buffer.
 * 
 * Parameter:
 *  Pointer to the affected buffer
 * 
 * Return:
 *  a variable from the buffer
 */
cbtype cbRead(cbuffer* buffer);
/**
 * add a new value to the buffer, adding may
 * fail depending on the buffer mode.
 * 
 * Parameter:
 *  Pointer to the affected buffer
 * 
 * Return:
 *  If the add was succesfull or not in a boolean
 */
bool cbAdd(cbuffer* buffer, cbtype value);

#endif
