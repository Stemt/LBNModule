#include "circularbuffer.h"
#include <stdlib.h>

cbuffer* cbInit(int8_t size, enum cbmode mode) {
  cbuffer* buffer = (cbuffer*) malloc(sizeof(cbuffer));
  if (!buffer) {
    return NULL;
  }
  buffer->data = (cbtype*) malloc(size * sizeof(cbtype));
  if(!buffer->data){
    free(buffer);
    return NULL;
  }
  buffer->mode = mode;
  buffer->size = size;
  buffer->start = 0;
  buffer->count = 0;

  return buffer;
}

cbuffer* cbFree(cbuffer* buffer) {
  if (buffer->data) {
    free(buffer->data);
  }
  if (buffer) {
    free(buffer);
  }
  return buffer;
}

bool cbAvailable(cbuffer* buffer) {
  if (buffer->count == 0) {
    return false;
  } else
    return true;
}

cbtype cbPeek(cbuffer* buffer) {
  return buffer->data[buffer->start];
}

cbtype cbRead(cbuffer* buffer) {
  cbtype cbTypeReturn = buffer->data[buffer->start];
  buffer->start = (buffer->start + 1) % buffer->size;
  buffer->count--;
  return cbTypeReturn;
}

bool cbAdd(cbuffer* buffer, cbtype value) {
  if (buffer->count < buffer->size) {
    buffer->data[(buffer->start + buffer->count) %
                 buffer->size] = value;
    buffer->count++;
  } else if (buffer->mode == OVERWRITE_IF_FULL) {
    buffer->data[buffer->start] = value;
    buffer->start = (buffer->start + 1) % buffer->size;
  } else if (buffer->mode == IGNORE_IF_FULL) {
    return false;
  }
  return true;
}
