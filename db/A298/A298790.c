/*
 * A298790  Partial sums of A298788.
 * https://oeis.org/A298790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1)+2*a(n-3)-2*a(n-4)-a(n-6)+a(n-7)  (n>6)
   a(0)=1, a(1)=4, a(2)=11, a(3)=23, a(4)=36, a(5)=53, a(6)=77 */

#include <stdlib.h>   /* malloc, realloc */

static const seq_t init_vals[7] = {
    (seq_t)1,
    (seq_t)4,
    (seq_t)11,
    (seq_t)23,
    (seq_t)36,
    (seq_t)53,
    (seq_t)77
};

static seq_t *cache_ptr = NULL;         /* dynamic array for cached terms */
static size_t cache_cap = 0;            /* allocated capacity of cache_ptr */
static size_t cache_size = 0;           /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* Fast return for the first 7 terms without touching the cache */
    if ((size_t)n < 7)
        return init_vals[(size_t)n];

    size_t idx = (size_t)n;

    /* Ensure we have enough space for the term at position idx */
    if (cache_cap <= idx) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= idx)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache_ptr, new_cap * sizeof(seq_t));
        if (tmp == NULL)     /* allocation failed */
            return (seq_t)0;
        cache_ptr = tmp;
        cache_cap = new_cap;
    }

    /* Make sure the first 7 values are stored before using the recurrence */
    if (cache_size < 7) {
        for (size_t k = cache_size; k < 7; ++k)
            cache_ptr[k] = init_vals[k];
        cache_size = 7;
    }

    /* Compute any missing terms up to idx */
    for (size_t k = cache_size; k <= idx; ++k) {
        /* a(k) = a(k-1) + 2*a(k-3) - 2*a(k-4) - a(k-6) + a(k-7) */
        cache_ptr[k] =
            cache_ptr[k - 1] +
            (seq_t)2 * cache_ptr[k - 3] -
            (seq_t)2 * cache_ptr[k - 4] -
            cache_ptr[k - 6] +
            cache_ptr[k - 7];
    }

    cache_size = idx + 1;  /* we now store terms 0 .. idx */

    return cache_ptr[idx];
}
