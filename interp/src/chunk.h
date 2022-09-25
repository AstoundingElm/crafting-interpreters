#pragma once
#include "defines.h"

#include "list_types.h"
#include <stdlib.h>

enum {OP_CONSTANT, OP_ADD, OP_SUBTRACT, OP_MULTIPLY,
      OP_DIVIDE, OP_NEGATE, OP_RETURN,};

typedef double Value;


struct Value_Array{

  i32 capacity;
  i32 count;
  Value * values;
  

};


struct Chunk{

  i32 count;
  i32 capacity;
  u8 * actual_array;
  i32 * lines;
  Value_Array constants;
  
};


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

PINLINE void init_value_array(Value_Array * array){

  array->values = NULL;
  array->capacity = 0;
  array->count = 0;

};


PINLINE void init_chunk(Chunk * chunk){

  chunk->count = 0;
  chunk->capacity = 0;
  chunk->actual_array = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);

};

PINLINE void write_chunk(Chunk * chunk, u8 byte, i32 line){

  if(chunk->capacity < chunk->count + 1){

    u32 old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->actual_array = GROW_ARRAY(u8, chunk->actual_array, old_capacity,chunk->capacity);
    chunk->lines = GROW_ARRAY(i32, chunk->lines, old_capacity, chunk->capacity);
    
  }

  chunk->actual_array[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;

}

PINLINE void write_value_array(Value_Array * array, Value value){

  if(array->capacity < array->count + 1){

    i32 old_capacity = array->capacity;
    array->capacity = GROW_CAPACITY(old_capacity);
    array->values = GROW_ARRAY(Value, array->values, old_capacity, array->capacity);
    
  }
  array->values[array->count] = value;
  array->count++;
  

};

PINLINE void free_value_array(Value_Array * array){

  FREE_ARRAY(Value, array->values, array->capacity);
  init_value_array(array);

};

PINLINE void free_chunk(Chunk * chunk ){

  FREE_ARRAY(u8, chunk->actual_array, chunk->capacity);
  FREE_ARRAY(i32, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);

}

PINLINE i32 add_constant(Chunk * chunk, Value value){

  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;

};
