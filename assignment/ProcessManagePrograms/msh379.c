#include <stdio.h>
#include <stdarg.h>

#include <sys/resource.h>
#include <sys/times.h>

#define SOFTLIMIT 300
#define HARDLIMIT 600


void WARNING (const char *fmt, ...) {
    va_list ap;
    
    fflush( stdout );
    
    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
}


int main() {
    
    const struct rlimit limit = {
        SOFTLIMIT,
        HARDLIMIT
    };

    if ( setrlimit( RLIMIT_CPU, &limit ) != 0 ) {
        WARNING( "Fail to set CPU time limit" );
    }

    struct tms tms_start;
    struct tms tms_end;
    clock_t wall_time = times( &tms_start );

    if ( wall_time == -1 ) {
        WARNING( "Fail to start recordiing user CPU time" );
    }

    printf("walltime : %ld\n", wall_time);

    return 0;
}