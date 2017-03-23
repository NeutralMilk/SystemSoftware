//
// Created by Timothy Barnard on 10/03/2017.
//

#ifndef ASSIGNMENT1_FILE_CHANGES_H
#define ASSIGNMENT1_FILE_CHANGES_H

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void check_folder( char * directory_name, int watcher, int notifyID ) ;

#endif //ASSIGNMENT1_FILE_CHANGES_H