/*
 * A281403  Number of 4Xn 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for A281403 a(n) = 4*a(n‑1) – 6*a(n‑2) +
 *            6*a(n‑3) – 5*a(n‑4) + 3*a(n‑6) – 2*a(n‑7) + 2*a(n‑8) – a(n‑10)
 *            (n > 16) Initial values (n = 1 … 16): 0, 36, 438, 1020, 2146,
 *            3736, 6276, 10740, 18472, 31360, 53204, 90516, 153772, 260532,
 *            440880, 745272
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 32, u64 n <= 77, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Shared lookup table for the first 16 terms */
static const seq_t init[16] = {
    (seq_t)0ULL,
    (seq_t)36ULL,
    (seq_t)438ULL,
    (seq_t)1020ULL,
    (seq_t)2146ULL,
    (seq_t)3736ULL,
    (seq_t)6276ULL,
    (seq_t)10740ULL,
    (seq_t)18472ULL,
    (seq_t)31360ULL,
    (seq_t)53204ULL,
    (seq_t)90516ULL,
    (seq_t)153772ULL,
    (seq_t)260532ULL,
    (seq_t)440880ULL,
    (seq_t)745272ULL
};

/* Memoised array of computed values.  Only grows to the maximum n
   requested so far.  Since the recurrence is linear and the
   coefficients fit in 64 bits, all intermediate calculations fit
   safely in the arbitrary‑precision type seq_t. */
static seq_t *cache = NULL;     /* pointer to stored terms */
static idx_t  cache_size = 0;   /* number of entries currently valid */

/* Obtain a value for a(n) with n ≥ 1.  The function is thread‑unsafe,
   but the problem statement does not require thread safety. */
seq_t a(idx_t n)
{
    if (n == 0u)               /* offset is 1, behaviour for n<1 ignored */
        return (seq_t)0;
    if (n <= 16u)              /* quick path for the first 16 terms */
        return init[n - 1u];

    /* Grow the cache if necessary */
    if (cache == NULL || cache_size < n) {
        idx_t new_size = n;
        seq_t *new_arr;
        if (cache == NULL) {
            new_arr = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
            if (!new_arr)
                return (seq_t)0;           /* allocation failure undefined */
            /* initialise the first 16 terms */
            for (idx_t i = 0u; i < 16u; ++i)
                new_arr[i] = init[i];
        } else {
            new_arr = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
            if (!new_arr)
                return (seq_t)0;           /* allocation failure undefined */
        }
        cache = new_arr;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = 16u; i < n; ++i) {
        /* cache[i] corresponds to a(i+1).  All the indices below are
           guaranteed to be valid because i ≥ 16. */
        seq_t value = 4u * cache[i - 1]
                    - 6u * cache[i - 2]
                    + 6u * cache[i - 3]
                    - 5u * cache[i - 4]
                    + 3u * cache[i - 6]
                    - 2u * cache[i - 7]
                    + 2u * cache[i - 8]
                    - cache[i - 10];
        cache[i] = value;
    }

    return cache[n - 1];
}
