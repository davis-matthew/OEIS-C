/*
 * A241937  Number of length 1+4 0..n arrays with no consecutive five elements
 *          summing to more than 2*n.
 * https://oeis.org/A241937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 112, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
   Initial values: a(1)=16, a(2)=96, a(3)=357, a(4)=1007, a(5)=2373, a(6)=4928 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;            /* largest index currently stored in cache */

/* Base values for n = 1..6, index 0 unused */
static const seq_t base[7] = {0, 16, 96, 357, 1007, 2373, 4928};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not needed for OEIS offset, but harmless */

    /* Ensure cache is allocated and contains values up to n */
    if (cache == NULL) {                 /* first call, create cache for at least 6 entries */
        size_t init_len = 7;              /* indices 0..6 inclusive */
        cache = (seq_t *)malloc((init_len + 1) * sizeof(seq_t));
        if (cache == NULL) abort();      /* out of memory, terminate */
        for (size_t i = 1; i <= 6; ++i) cache[i] = base[i];
        cache_len = 6;                   /* highest stored index */
    }

    if ((size_t)n > cache_len) {         /* need to extend cache */
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();        /* out of memory */
        cache = tmp;
        for (size_t i = cache_len + 1; i <= new_len; ++i) {
            if (i <= 6) {
                cache[i] = base[i];
            } else {
                /* Compute recurrence with only non‑negative intermediate values
                 * to avoid unsigned underflow. */
                seq_t s1 = cache[i - 1] + cache[i - 5];
                s1 *= (seq_t)6;
                s1 += cache[i - 3] * (seq_t)20;

                seq_t s2 = cache[i - 2] + cache[i - 4];
                s2 *= (seq_t)15;
                s2 += cache[i - 6];

                cache[i] = s1 - s2;        /* s1 >= s2 by combinatorial positivity */
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
