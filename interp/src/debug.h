#pragma once
#include <stdio.h>
#include "defines.h"
#include "chunk.h"

#define PRINT_ARRAY(array)   printf("count: %d.\n capacity: \
 %d.\n actual _array: %zd.\n", array.count, array.capacity, \
				    (size_t)array.actual_array) \


PINLINE i32 simple_instruction(const b8 * name, i32 offset){

  printf("%s\n", name);
  return offset + 1;
  

};

PINLINE void print_value(Value value){

  printf("%g", value);
}

PINLINE i32 constant_instruction(const b8 * name, Chunk * chunk, i32 offset){

  u8 constant = chunk->actual_array[offset + 1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
	      printf("'\n");
	      return offset +2;
}

PINLINE i32 dissasemble_instruction(Chunk * chunk, int offset){

  printf("%04d ", offset);
  if(offset > 0 && chunk->lines[offset] == chunk->lines[offset -1]){

      printf("  |  ");
      
    }else{

      printf("%4d", chunk->lines[offset]);
      
    }
  u8 instruction = chunk->actual_array[offset];
  switch(instruction){
  case OP_CONSTANT:
    return constant_instruction("OP_CONSTANT", chunk, offset);
  case OP_ADD: return simple_instruction("OP_ADD", offset);
    case OP_SUBTRACT: return simple_instruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY: return simple_instruction("OP_MULTIPLY", offset);
  case OP_DIVIDE: return simple_instruction("OP_DIVIDE", offset);
  case OP_NEGATE: return simple_instruction("OP_NEGATE", offset);
  case OP_RETURN:
    return simple_instruction("OP_RETURN", offset);
    
  default:
    printf("unknown opcode %d\n", instruction);
    return offset + 1;
  }
  
};

PINLINE void dissasemble_chunk(Chunk * chunk, const char * name){

  printf("== %s ==\n", name);

  for(i32 offset = 0; offset < chunk->count;){

    offset = dissasemble_instruction(chunk, offset);

  } 
  
  

};





