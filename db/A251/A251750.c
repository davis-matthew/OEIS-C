/*
 * A251750  9-step Fibonacci sequence starting with 0,0,0,1,0,0,0,0,0.
 * https://oeis.org/A251750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 137,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 9‑step Fibonacci:  a(0)=0,a(1)=0,a(2)=0,a(3)=1,a(4)=0,a(5)=0,a(6)=0,a(7)=0,a(8)=0
    Recurrence:  a(n+9)=a(n)+a(n+1)+…+a(n+8)   for n ≥ 0                          */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Initial 9 terms */
    static const seq_t init[9] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1,
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)0
    };

    /* Cached sequence */
    static seq_t *cache      = NULL;  /* storage for a(0)..a(cache_len-1) */
    static size_t cache_len  = 0;      /* number of elements stored */
    static size_t cache_cap  = 0;      /* current allocated capacity */

    /* Determine how many elements have to be guaranteed for the current call */
    size_t need = (size_t)((idx_t)8 <= n ? 9 : (size_t)n + 1);

    /* Ensure we have memory for those elements */
    if (need > cache_cap)
        ensure_capacity(need, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Initialise the cache if this is the first call or we did a resize */
    if (cache_len == 0)
    {
        /* Copy the known initial terms that are needed for this `n` */
        size_t i;
        for (i = 0; i < 9 && i <= (size_t)n; ++i)
            cache[i] = init[i];

        /* Do not write beyond what the caller actually needs */
        cache_len = (size_t)((idx_t)8 <= n ? 9 : (size_t)n + 1);
    }

    /* If the caller requests a term that is already cached return it */
    if ((idx_t)n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Otherwise compute missing terms up to `n` */
    /* First, make sure we have room for all terms up to n */
    size_t total_needed = (size_t)n + 1;
    if (total_needed > cache_cap)
        ensure_capacity(total_needed, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute each new term using the recurrence */
    for (idx_t k = (idx_t)cache_len; k <= n; ++k)
    {
        if (k < (idx_t)9) {
            /* Already initialised */
            cache[(size_t)k] = init[k];
        } else {
            seq_t sum = (seq_t)0;
            idx_t off;
            for (off = 1; off <= 9; ++off)
                sum += cache[(size_t)(k - off)];
            cache[(size_t)k] = sum;
        }
    }

    /* Update the length of the cache */
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
