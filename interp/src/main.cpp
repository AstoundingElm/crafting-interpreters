
#include "defines.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
PINLINE b8 * read_file(const b8 * path){

  FILE * file = fopen(path, "rb");
  if(file == NULL){

    fprintf(stderr, "could not open file \"%s\".\n", path);
    exit(74);
  }
  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  b8 * buffer = (b8 *)malloc(file_size + 1);
  if(buffer == NULL){
    fprintf(stderr, "not enugh memory to read \"%s\".\n", path);
    exit(74);

  }
  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if(bytes_read < file_size){
    fprintf(stderr, "could not read file \"%s\".\n", path);
    exit(74);
  }
  buffer[bytes_read] = '\0';
  fclose(file);
  return buffer;

}

PINLINE void run_file(const b8 * path){

  b8 * source = read_file(path);
  INTERPRETRESULT result = interpret(source);
  free(source);

  if(result == INTERPRET_COMPILE_ERROR) exit(65);
  if(result == INTERPRET_RUNTIME_ERROR) exit(70);
  
};

PINLINE void repl(){

  b8 line[1024];
  for(;;){

    printf("> ");

    if(!fgets(line, sizeof(line), stdin)){

      printf("\n");
      break;
    }

    interpret(line);
  };
  
  
}

int main(int argc, const char * argv[]){

  init_vm();
  /* Chunk chunk;
  PRINT_ARRAY(chunk);
  init_chunk(&chunk);

  i32 constant  = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 123);
  
  write_chunk(&chunk, constant, 123);
  constant = add_constant(&chunk, 3.4);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant, 123);

write_chunk(&chunk, OP_ADD, 123);
constant = add_constant(&chunk, 5.6);
write_chunk(&chunk, OP_CONSTANT, 123);
write_chunk(&chunk, constant, 123);
write_chunk(&chunk, OP_DIVIDE, 124);
  
  write_chunk(&chunk, OP_NEGATE, 123);
  write_chunk(&chunk, OP_RETURN, 123);
  PRINT_ARRAY(chunk);

  // dissasemble_chunk(&chunk, "test chunk");
  interpret(&chunk);*/
  if(argc == 1){

    repl();
  }else if(argc == 2){
    run_file(argv[1]);
  }else{
    fprintf(stderr, "usage: clox [path] \n");
    exit(64);
  }
  free_vm();
  
};
