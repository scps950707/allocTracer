/*
 * Author:         scps950707
 * Email:          scps950707@gmail.com
 * Created:        2018-06-18 13:19
 * Last Modified:  2018-06-21 16:36
 * Filename:       malloc.c
 * Description:    Use LD_PRELOAD to track malloc free sequences
 */

#include<stdio.h>
#include<unistd.h>
#include<dlfcn.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#define LOGFNAME "logfile.txt"

int logFd = -1;
static void *( *real_malloc )( size_t ) = NULL;
static void ( *real_free )( void * ) = NULL;
#if 0
static void *( *real_calloc )( size_t, size_t ) = NULL;
#endif
static void *( *real_realloc )( void *, size_t ) = NULL;
#if 0
static void *( *real_valloc )( size_t ) = NULL;
static void *( *real_memalign )( size_t, size_t ) = NULL;
static int ( *real_posix_memalign )( void **, size_t, size_t ) = NULL;
static void *( *real_aligned_alloc )( size_t, size_t ) = NULL;
#endif


/* called when shared library is loaded */
static void __attribute__ ( ( constructor ) )init( void )
{
	if ( ( logFd = open( LOGFNAME, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) < 0 )
	{
		fprintf( stderr, "error opening file: %s\n", strerror( errno ) );
	}
}

static void __attribute__ ( ( destructor ) )fini( void )
{
	close( logFd );
}

static void LOG( const char *op, size_t sz, void *ptr )
{
	if ( logFd > 0 )
	{
		char buf[64];
		sprintf( buf, "%s %d %p\n", op, ( int )sz, ptr );
		write( logFd, buf, strlen( buf ) );
	}
}


void *malloc( size_t size )
{
	if ( real_malloc == NULL )
	{
		real_malloc = ( void *( * )( size_t ) )dlsym( RTLD_NEXT, "malloc" );
	}
	void *ptr = real_malloc( size );
	if ( ptr != NULL && size > 0 )
	{
		LOG( "malloc", size, ptr );
	}
	return ptr;
}

void free( void *ptr )
{
	if ( real_free == NULL )
	{
		real_free = ( void( * )( void * ) )dlsym( RTLD_NEXT, "free" );
	}
	LOG( "free", 0, ptr );
	real_free( ptr );
}

// TODO still some problems cause crash
// https://stackoverflow.com/a/46175238
// http://blog.bigpixel.ro/interposing-calloc-on-linux/
// https://stackoverflow.com/a/10008252
#if 0
static void *temporary_calloc( size_t nmemb, size_t size )
{
	printf( "empty calloc called\n" );
	void *ptr = malloc( nmemb * size );
	memset( ptr, 0, nmemb * size );
	return ptr;
}

void *calloc( size_t nmemb, size_t size )
{
	if ( real_calloc == NULL )
	{
		// before trying to get the libc provided
		// calloc, set the calloc function to the temporary
		// one returning NULL.
		real_calloc = temporary_calloc;

		// dlsym will call again this calloc in which we are,
		// but seems like it handles just fine if we are
		// returning NULL
		real_calloc = ( void *( * )( size_t, size_t ) ) dlsym( RTLD_NEXT, "calloc" );
	}
	void *ptr = real_calloc( nmemb, size );
	if ( ptr != NULL && size > 0 )
	{
		LOG( "malloc", nmemb * size, ptr );
	}
	return ptr;
}
#endif

void *realloc( void *ptr, size_t size )
{
	if ( real_realloc == NULL )
	{
		real_realloc = ( void *( * )( void *, size_t ) )dlsym( RTLD_NEXT, "realloc" );
	}
	void *nptr = real_realloc( ptr, size );
	if ( nptr != NULL && size > 0 )
	{
		/* LOG( "realloc", size, nptr ); */
		LOG( "free", 0, ptr );
		LOG( "malloc", size, nptr );
	}
	return nptr;
}

#if 0
void *valloc( size_t size )
{
	if ( real_valloc == NULL )
	{
		real_valloc = ( void *( * )( size_t ) )dlsym( RTLD_NEXT, "valloc" );
	}
	void *ptr = real_valloc( size );
	if ( ptr != NULL && size > 0 )
	{
		/* LOG( "valloc", size, ptr ); */
		LOG( "malloc", size, ptr );
	}
	return ptr;
}

void *memalign( size_t alignment, size_t size )
{
	if ( real_memalign == NULL )
	{
		real_memalign = ( void *( * )( size_t, size_t ) )dlsym( RTLD_NEXT, "memalign" );
	}
	void *ptr = real_memalign( alignment, size );
	if ( ptr != NULL && size > 0 )
	{
		/* LOG( "memalign", size, ptr ); */
		LOG( "malloc", size, ptr );
	}
	return ptr;
}

int posix_memalign( void **memptr, size_t alignment, size_t size )
{
	if ( real_posix_memalign == NULL )
	{
		real_posix_memalign = ( int( * )( void **, size_t, size_t ) )dlsym( RTLD_NEXT, "posix_memalign" );
	}
	int ret = real_posix_memalign( memptr, alignment, size );
	if ( memptr != NULL && size > 0 )
	{
		/* LOG( "posix_memalign", size, memptr ); */
		LOG( "malloc", size, memptr );
	}
	return ret;
}

void *aligned_alloc( size_t alignment, size_t size )
{
	if ( real_aligned_alloc == NULL )
	{
		real_aligned_alloc = ( void *( * )( size_t, size_t ) )dlsym( RTLD_NEXT, "aligned_alloc" );
	}
	void *ptr = real_aligned_alloc( alignment, size );
	if ( ptr != NULL && size > 0 )
	{
		/* LOG( "aligned_alloc", size, ptr ); */
		LOG( "malloc", size, ptr );
	}
	return ptr;
}
#endif
