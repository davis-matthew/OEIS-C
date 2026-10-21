/*
 * A294325  a(n) = Product_{k=0..n} (5*k + 3)!.
 * https://oeis.org/A294325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (5*k + 3)!
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses incremental factorials.
   Each new factorial (5*k+3)! is obtained by multiplying the
   previous factorial (5*(k-1)+3)! by the next five integers.
   The sequence values a(n) are stored in a static cache so that
   calls with arbitrary order of n remain efficient. */

#include <stdlib.h>
#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* static cache of a(k) values */
    static seq_t *cache       = NULL;
    static size_t cache_cap   = 0;
    static idx_t   cache_len  = 0;

    /* keeps the current factorial (5*(k-1)+3)! while filling cache */
    static seq_t curr_fact = 0;

    /* initialise cache on first use */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap); /* cast for C++ */
    }

    /* grow the cache as needed */
    while (cache_len <= n)
    {
        if (cache_len == 0)
        {
            /* first term: (5*0+3)! = 3! */
            curr_fact = factorial( (idx_t)3 );
            cache[0]  = curr_fact;
        }
        else
        {
            /* extend factorial to (5*k+3)! */
            idx_t k = cache_len;              /* current index to compute */
            idx_t start = 5 * k - 1;          /* first multiplier */
            idx_t end   = 5 * k + 3;          /* last multiplier */

            for (idx_t m = start; m <= end; ++m)
            {
                curr_fact = seq_mul(curr_fact, (seq_t)m);
            }

            /* compute a(k) = a(k-1) * (5*k+3)! */
            cache[k] = seq_mul( cache[k-1], curr_fact );
        }

        ++cache_len;

        /* resize cache if it is full */
        if (cache_len == (idx_t)cache_cap)
        {
            cache_cap <<= 1;
            cache = (seq_t *)realloc( cache, sizeof(seq_t) * cache_cap ); /* cast for C++ */
        }
    }

    return cache[(size_t)n];
}
