/*
 * A263119  Number of (n+3) X (1+3) 0..1 arrays with each row divisible by 15
 *          and column not divisible by 15, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A263119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A263119
 *
 * Empirical recurrence:
 *   a(1)=14, a(2)=29, a(3)=59, a(4)=119, a(5)=238
 *   a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5)   for n ≥ 6
 *
 * The function `a` returns the n‑th term for any n ≥ 1.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for memoisation.  cache[0] is unused. */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static idx_t cache_size = 0;     /* number of allocated slots (including 0) */
static idx_t cache_max = 0;      /* highest index for which a value is stored */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0. */
    if (n < 1)
        return 0;

    /* Ensure cache can hold index n. */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                /* allocation failed */
            return 0;
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Initialise first five terms if not already done. */
    if (cache_max < 5) {
        if (cache_size < 6) { /* need room up to index 5 */
            size_t required = 6;
            seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
            if (!tmp)
                return 0;
            cache = tmp;
            cache_size = (idx_t)required;
        }
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)29;
        cache[3] = (seq_t)59;
        cache[4] = (seq_t)119;
        cache[5] = (seq_t)238;
        cache_max = 5;
    }

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + a(i-4) - 2*a(i-5) */
        seq_t val = cache[i - 1] * (seq_t)2
                    + cache[i - 4]
                    - (seq_t)2 * cache[i - 5];
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
