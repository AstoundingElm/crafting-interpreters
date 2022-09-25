#pragma once

#include "defines.h"
#include "chunk_types.h"
#include "chunk.h"
#include <stdlib.h>



#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)



PINLINE void * reallocate(void * pointer, size_t old_size, size_t new_size){

  if(new_size ==0)
    {
      free(pointer);
      return NULL;
    };

  void * result = realloc(pointer, new_size);
  if(result == NULL) exit(1);
  return result;
  
  
};



#define GROW_ARRAY(type, pointer, old_count, new_count) \
  (type *)reallocate(pointer, sizeof(type) * (old_count),	\
       sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count) \
  reallocate(pointer, sizeof(type) * (old_count), 0)

PINLINE void free_chunk(Chunk * chunk ){

  FREE_ARRAY(u8, chunk->actual_array, chunk->capacity);
  init_chunk(chunk);

}

