/**
 * Parse the PostgreSQL code in the file given as parameter and print
 * the JSON generated in stdout.
 *
 * To compile (from root directory of this project):
 * cc -I. -L. -o examples/parse_file -g examples/parse_file.c libpg_query.a
 */

#include <pg_query.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  PgQueryParseResult result;
  size_t i;
  FILE * fp;
  char * buffer = NULL;
  size_t len;
  ssize_t bytes_read;
  if ( argc != 2){
      printf("No file given as parameter.\n");
      return -1;
  }
  fp = fopen(argv[1], "r");
  if ( fp == NULL){
      printf("Could not open the file %s.\n", argv[1]);
      return -1;
  }
  bytes_read = getdelim( &buffer, &len, '\0', fp);
  if ( bytes_read != -1) {
    pg_query_init();

    result = pg_query_parse(buffer);

    if (result.error) {
      printf("error: %s at %d\n", result.error->message, result.error->cursorpos);
    } else {
      printf("%s\n", result.parse_tree);
    }

    pg_query_free_parse_result(result);

    return 0;
  } else {
    printf("Error: Could not parse the file.");
  }
}
