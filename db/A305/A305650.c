/*
 * A305650  a(n) = -1/3 * (u^n-1)*(v^n-1) with u = 1+sqrt(3), v = 1-sqrt(3).
 * https://oeis.org/A305650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(3)=9, a(4)=13; a(n)=a(n-1)+6*a(n-2)-2*a(n-3)-4*a(n-4) for n>4. */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array holding computed values */
static size_t cache_cap = 0;     /* current capacity of cache */

/* Compute A305650 term at index n (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) {          /* undefined for offset 1, but safe */
        return (seq_t)0;
    }

    /* Handle small n directly */
    if (n <= 4) {
        switch ((size_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)1;
            case 3: return (seq_t)9;
            case 4: return (seq_t)13;
        }
    }

    /* Convert idx_t to size_t for array indexing */
    size_t N = (size_t)n;
    size_t needed = N + 1;          /* indices 0..N inclusive */

    /* Allocate or grow cache as needed */
    if (cache_cap < needed) {
        size_t new_cap = needed;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        }
        /* Copy old values if any */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
        /* Initialise initial terms if cache was just created */
        if (cache_cap >= 5) {
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)9;
            cache[4] = (seq_t)13;
        }
    }

    /* Compute missing values up to N */
    for (size_t i = 5; i <= N; ++i) {
        if (cache[i] != 0) continue;          /* already computed */
        seq_t temp = cache[i-1] + (seq_t)6 * cache[i-2];
        seq_t sub  = (seq_t)2 * cache[i-3] + (seq_t)4 * cache[i-4];
        cache[i] = temp - sub;
    }

    return cache[N];
}
