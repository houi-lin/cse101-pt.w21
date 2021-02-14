//-----------------------------------------------------------------------------
// WordFrequency.c
// Determines frequency of each word in a file.
//
// Usage:  WordFrequency <input file> <output file>
//
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"Dictionary.h"

#define MAX_LINE_LEN 300


int main(int argc, char* argv[]){

   Dictionary D = newDictionary(1); // unique keys
   FILE* in = NULL;
   FILE* out = NULL;
   char delim[] = "\x20\n\t\r\\\"\',<.>/?;:[{]}|`~!@#$^&*()-_=+0123456789"\
                  "\xE2\x80\x98\x94\x99\x9C\x9D\x25";
   char line[MAX_LINE_LEN];
   char** keyBuffer = NULL;
   int**  valBuffer = NULL;
   char* word;
   char* ch;
   int* p;
   int i, n=0;

   // check command line arguments 
   if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // open input file 
   in = fopen(argv[1], "r");
      if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   // open output file
   out = fopen(argv[2], "w");
   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }

   // read each line of input 
   while( fgets(line, MAX_LINE_LEN, in) != NULL)  {
      
      // get each word on this line
      word = strtok(line, delim);
      while( word!=NULL ){ // there is another word
         
         // convert word to lower case
         for(ch=word; *ch!='\0'; ch++){
            *ch = tolower(*ch);
         }

         p = lookup(D, word);
         if( p==VAL_UNDEF ){ // this word is new
            n++;

            keyBuffer = realloc(keyBuffer, n*sizeof(KEY_TYPE));
            keyBuffer[n-1] = calloc(strlen(word)+1, sizeof(char));
            strcpy(keyBuffer[n-1], word);

            valBuffer = realloc(valBuffer, n*sizeof(VAL_TYPE));
            valBuffer[n-1] = malloc(sizeof(int));
            *valBuffer[n-1] = 1;

            insert( D, keyBuffer[n-1], valBuffer[n-1] );

         }else{ // this word has been seen already
            (*p)++;
         }

         // get next token
         word = strtok(NULL, delim);
      }
   }

   // print words and their frequencies
   fprintf(out, " word                frequency\n");
   fprintf(out, "-----------------------------------\n");
   for(p=beginForward(D); p!=VAL_UNDEF; p=next(D)) {
      fprintf(out, " %-20s%d\n", currentKey(D), *currentVal(D));
   }
   fprintf(out,"\n");

   // free resources
   freeDictionary(&D);
   for(i=0; i<n; i++){
      free(keyBuffer[i]);
      free(valBuffer[i]);
   }
   free(keyBuffer);
   free(valBuffer);
   fclose(in);
   fclose(out);

   return(0);
}
