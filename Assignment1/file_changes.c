/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/inotify.h>

#include "file_changes.h"
#include "logger.h"

void check_folder( char * directory_name, int watcher, int notifyID ) 
{
  int length, i = 0;
  char buffer[EVENT_BUF_LEN];

  length = read( notifyID, buffer, EVENT_BUF_LEN ); 

  /*checking for error*/
  if ( length < 0 ) {
    perror( "read" );
  }  

  /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
  while ( i < length ) {  struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) {
          //printf( "New directory %s created in %s .\n", event->name, directory_name );
          log_data_two("CREATED directory: ",  event->name );
        }
        else {
          //printf( "New file %s created in %s .\n", event->name, directory_name );
          log_data_two("CREATED file: ",  event->name );
        }
      }
      else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          //printf( "Directory %s deleted in %s .\n", event->name,directory_name  );
          log_data_two("DElETED directory: ",  event->name );
        }
        else {
          //printf( "File %s deleted %s .\n", event->name, directory_name );
          log_data_two("DELETED file: ",  event->name );
        }
      }
    }
    i += EVENT_SIZE + event->len;
  }
}