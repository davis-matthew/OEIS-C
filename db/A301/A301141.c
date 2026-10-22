/*
 * A301141  Coordination sequence T2 for Zeolite Code NSI.
 * https://oeis.org/A301141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=12, a(3)=26, a(4)=43, a(5)=64, a(6)=99,
 *            a(7)=133, a(8)=161, a(9)=210, a(10)=274, a(11)=318, a(12)=364,
 *            a(13)=441, a(14)=528. For n>14: a(n) = -2*a(n-2) + 2*a(n-3) +
 *            4*a(n-5) + a(n-6) - a(n-8) - 4*a(n-9) - 2*a(n-11) + 2*a(n-12) +
 *            a(n-14)
 * Range:     u8 n <= 9, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[15] = {
    (seq_t)1,  (seq_t)4,  (seq_t)12, (seq_t)26, (seq_t)43,
    (seq_t)64, (seq_t)99, (seq_t)133, (seq_t)161, (seq_t)210,
    (seq_t)274, (seq_t)318, (seq_t)364, (seq_t)441, (seq_t)528
};

static seq_t *cache = NULL;
static idx_t  cap   = 0;
static idx_t  max   = (idx_t)-1;   /* sentinel: cache not yet initialized */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 15) return init_vals[n];

    /* Initialize cache on first call or when needed */
    if (cache == NULL) {
        /* Start with space for at least 16 elements */
        cap = 16;
        if (cap < n + 1) {
            /* Ensure capacity for current request */
            while (cap <= n) cap <<= 1;
        }
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        /* Copy known initial values */
        for (idx_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
        max = 14;
    }

    /* Expand capacity if current request is beyond current cache */
    if (n > max) {
        if (n + 1 > cap) {
            size_t new_cap = cap;
            while (new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* reallocation failed */
            cache = tmp;
            cap   = (idx_t)new_cap;
        }

        /* Compute terms up to n sequentially */
        for (idx_t i = max + 1; i <= n; ++i) {
            seq_t term = 0;

            /* Positive contributions */
            term += (seq_t)2 * cache[i - 3];
            term += (seq_t)4 * cache[i - 5];
            term += cache[i - 6];
            term += (seq_t)2 * cache[i - 12];
            term += cache[i - 14];

            /* Negative contributions */
            term -= (seq_t)2 * cache[i - 2];
            term -= cache[i - 8];
            term -= (seq_t)4 * cache[i - 9];
            term -= (seq_t)2 * cache[i - 11];

            cache[i] = term;
        }
        max = n;
    }

    return cache[n];
}
