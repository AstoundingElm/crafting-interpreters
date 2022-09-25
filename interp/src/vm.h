#pragma once

#include "defines.h"
#include "chunk.h"

#define STACK_MAX 256

struct VM{

  Chunk * chunk;
  u8 * ip;
  Value stack[STACK_MAX];
  Value * stack_top;

};

VM vm;

enum INTERPRETRESULT{

  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,

  
};


PINLINE void push(Value value){

  *vm.stack_top = value;
  vm.stack_top++;

};

PINLINE Value pop(){

  vm.stack_top--;
  return *vm.stack_top;

}


PINLINE INTERPRETRESULT run(){

  
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

#define BINARY_OP(op) \
  do{ \
    double b = pop(); \
    double a  = pop();				\
      push(a op b);   \
      }while (false)
  
  for(;;){
    #ifdef DEBUG_TRACE_EXECUTION
    printf("           ");
    for(Value * slot = vm.stack; slot < vm.stack_top; slot++){

      printf("[ ");
      print_value(*slot);
      printf(" ]");
      

    }

    printf("\n");
    
    dissasemble_instruction(vm.chunk, (i32)(vm.ip - vm.chunk->actual_array));
    #endif

    
    
    u8 instruction;
    switch(instruction = READ_BYTE()){
    case OP_CONSTANT:{

      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_ADD: BINARY_OP(+); break;
    case OP_SUBTRACT: BINARY_OP(-); break;
    case OP_MULTIPLY: BINARY_OP(*); break;
    case OP_DIVIDE: BINARY_OP(/); break;
    case OP_NEGATE: push(-pop()); break;
    case OP_RETURN:{

      print_value(pop());
      printf("\n");
      
      return INTERPRET_OK;
    }

    };
  }

    #undef READ_BYTE
  #undef READ_CONSTANT
  #undef BINARY_OP
    
};

PINLINE void reset_stack(){vm.stack_top = vm.stack;}

PINLINE void init_vm(){
  reset_stack();

};

PINLINE void free_vm(){};

PINLINE INTERPRETRESULT interpret(Chunk * chunk){


  vm.chunk = chunk;
  vm.ip = vm.chunk->actual_array;
  return run();
  
};
