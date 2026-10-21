/*
 * A246413  Positive integers k satisfying csc(k) > csc(k+1).
 * https://oeis.org/A246413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   produce the nth integer k for which 1/sin(k) > 1/sin(k+1)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* The sequence is generated lazily and cached across calls. */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cached_size = 0;   /* number of terms stored */
static size_t cache_capacity = 0;/* current allocation size */
static idx_t next_k = 1;         /* next integer to test */

/* Helper to grow the cache as needed. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();   /* out of memory */
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /*  Generate terms until we have at least n entries. */
    while (cached_size < n)
    {
        /* Compute sin(k) and sin(k+1) using long double for precision. */
        long double sin_k  = sinl((long double)next_k);
        long double sin_k1 = sinl((long double)(next_k + 1));

        /* Determine whether 1/sin(k) > 1/sin(k+1) */
        int holds;
        if (sin_k > 0.0L && sin_k1 < 0.0L)
            holds = 1;                     /* sign flips from + to - */
        else if (sin_k * sin_k1 > 0.0L && sin_k1 > sin_k)
            holds = 1;                     /* same sign and increase */
        else
            holds = 0;                     /* otherwise inequality fails */

        if (holds)
        {
            grow_cache(cached_size + 1);
            cache[cached_size++] = (seq_t)next_k;
        }

        ++next_k;  /* move to next integer to test */
    }

    return cache[n - 1];
}
