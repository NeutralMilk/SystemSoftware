#include <stdio.h>
#include <stdlib.h>
 
int main()
{
   char ch; 
   char source_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/tim.txt";
   char target_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/folder/tim.txt";
   FILE *source, *target;
 
   printf("Enter name of file to copy\n");
 
   source = fopen(source_file, "r");
 
   if( source == NULL )//
   {
      printf("Press any key to exit...\n");
      exit(EXIT_FAILURE);
   }
 
   target = fopen(target_file, "w");
 
   if( target == NULL )
   {
      fclose(source);
      printf("Press any key to exit...\n");
      exit(EXIT_FAILURE);
   }
 
   printf("File copied successfully.\n");
 
   fclose(source);
   fclose(target);
 
   return 0;
}