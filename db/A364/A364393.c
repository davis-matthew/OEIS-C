/*
 * A364393  G.f. satisfies A(x) = 1 + x*(1 + 1/A(x)^2).
 * https://oeis.org/A364393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 12, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364393
 *  Recurrence (taken from OEIS):
 *   2*n*(2*n-1)*a[n]
 * + (35*n^2-64*n+24)*a[n-1]
 * + (-35*n^2+205*n-288)*a[n-2]
 * + 2*(-43*n^2+341*n-660)*a[n-3]
 * - 4*(7*n-30)*(n-5)*a[n-4]
 * - 8*(n-5)*(n-6)*a[n-5] = 0
 *  Initial values: a[0]=1, a[1]=2, a[2]=-4, a[3]=20, a[4]=-120, a[5]=800
 *
 * The recurrence is evaluated using 128‑bit intermediates to avoid
 * overflow when seq_t is 64‑bit; the final result is cast back to seq_t.
 * Subsequent calls are memoised in a static array that grows as needed.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array of a(i) */
    static size_t cache_len = 0;     /* allocated length of cache   */
    static idx_t computed = 0;       /* next index that has not been computed (i.e. a(i) is unknown) */

    /* initialise cache on first call – allocate at least 64 slots */
    if (cache_len == 0) {
        size_t init_len = 64;
        while (init_len <= (size_t)n) init_len <<= 1;
        cache = seq_alloc_new_array(init_len);
        for (size_t i = 0; i < init_len; ++i) cache[i] = 0;
        /* set the first six values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)-4;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)-120;
        cache[5] = (seq_t)800;
        cache_len = init_len;
        computed = 6;              /* indices 0..5 have been initialised */
    }

    /* if requested index is already known, return it */
    if ((idx_t)n < computed) return cache[n];

    /* make sure the cache can hold up to index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = cache_len ? cache_len << 1 : 64;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *new_cache = seq_alloc_new_array(new_len);
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        __int128 N   = (__int128)i;
        __int128 N2  = N * N;

        __int128 term1 = ( (__int128)35 * N2 - (__int128)64 * N + (__int128)24 );
        __int128 term2 = ( -(__int128)35 * N2 + (__int128)205 * N - (__int128)288 );
        __int128 term3 = ( (__int128)2 * ( -(__int128)43 * N2 + (__int128)341 * N - (__int128)660 ) );
        __int128 term4 = ( -(__int128)4 * ( (__int128)7 * N - (__int128)30 ) * ( N - (__int128)5 ) );
        __int128 term5 = ( -(__int128)8 * ( N - (__int128)5 ) * ( N - (__int128)6 ) );

        __int128 num  = term1 * (__int128)cache[i-1]
                       + term2 * (__int128)cache[i-2]
                       + term3 * (__int128)cache[i-3]
                       + term4 * (__int128)cache[i-4]
                       + term5 * (__int128)cache[i-5];

        __int128 denom = (__int128)2 * N * ((__int128)2 * N - (__int128)1);

        __int128 val = -(num / denom);          /* exact division */

        cache[i] = (seq_t)val;
    }

    computed = n + 1;   /* next index to compute starts here */
    return cache[n];
}
