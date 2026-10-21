/*
 * A251765  10-step Fibonacci sequence starting with 0,0,1,0,0,0,0,0,0,0.
 * https://oeis.org/A251765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 10‑step Fibonacci (order 10) a(n) = a(n-1)+…+a(n-10)  */
/* initial terms: a(0)=0,a(1)=0,a(2)=1,a(3)=0,a(4)=0,a(5)=0,a(6)=0,a(7)=0,a(8)=0,a(9)=0 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    [0] = 0, [1] = 0, [2] = 1, [3] = 0, [4] = 0, [5] = 0, [6] = 0, [7] = 0, [8] = 0, [9] = 0
};

/* cached terms */
static seq_t *cache = NULL;      /* pointer to stored values */
static size_t cache_size = 0;    /* number of values stored */
static size_t cache_cap   = 0;    /* capacity of the cache */

/* return a(n) */
seq_t a(idx_t n)
{
    /* handle initial range directly */
    if (n < 10)
        return init_vals[n];

    /* allocate and initialise cache if needed */
    if (!cache) {
        cache_cap   = 10;
        cache_size  = 10;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
    }

    /* extend cache up to n if necessary */
    while (cache_size <= (size_t)n) {
        /* ensure enough capacity */
        if (cache_size == cache_cap) {
            cache_cap = (cache_cap == 0) ? 10 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure; abort (undefined behavior) */
                abort();
            }
            cache = tmp;
        }

        /* compute a(cache_size) as sum of previous 10 terms */
        seq_t val = 0;
        for (idx_t j = (idx_t)cache_size - 10; j < (idx_t)cache_size; ++j)
            val += cache[j];

        cache[cache_size++] = val;
    }

    return cache[(size_t)n];
}
