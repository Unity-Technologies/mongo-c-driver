/* timeouts.c */

#include "test.h"
#include "mongo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {

    mongo_connection conn[1];
    bson_buffer bb;
    bson b;
    int res;
    time_t t1, t2;

    if( mongo_connect( conn, TEST_SERVER, 27017 ) != MONGO_OK ) {
        printf("Failed to connect");
        exit(1);
    }

    res = mongo_simple_str_command( conn, "test", "$eval",
            "for(i=0; i<100000; i++) { db.foo.find() }", &b );

    ASSERT( res == MONGO_OK );

    /* 50ms timeout */
    mongo_conn_set_timeout( conn, 50 );

    ASSERT( conn->err == 0 );
    res = mongo_simple_str_command( conn, "test", "$eval",
            "for(i=0; i<100000; i++) { db.foo.find() }", &b );

    ASSERT( res == MONGO_ERROR );
    ASSERT( conn->err == MONGO_IO_ERROR );

    return 0;
}
