#include "test.h"
#include <stdlib.h>

int main( void )
{
	for ( int i = 0; i < seq_num; i++ )
	{
		if ( mem_ops[i] == 1 )
		{
			allocate_array[alloc_idx[i]] = ( long )malloc( alloc_size[i] );
		}
		else
		{
			free( ( void * )allocate_array[alloc_idx[i]] );
		}
	}
	return 0;
}
