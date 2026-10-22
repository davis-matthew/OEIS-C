/*
 * A352279  a(0) = 1; a(n) = 2 * Sum_{k=0..floor((n-1)/2)} binomial(n-1,2*k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 * Sum_{k=0..floor((n-1)/2)} binomial(n-1,2*k)
 *            * a(n-2*k-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

/* Returns the n-th term of OEIS sequence A352279. */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t  next   = 1;      /* index that has not yet been computed */
    static size_t cap    = 0;      /* allocated capacity of the cache */

    /* Ensure the cache is large enough for index n. */
    if (cap <= (size_t)n) {
        /* expand cache up to n+1 elements */
        void *tmp = realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation failed; abort gracefully */
            return (seq_t)0;
        }
        cache = (seq_t *)tmp;
        cap   = n + 1;
    }

    /* initialise base case if this is the first call */
    if (next == 1 && cache[0] != 1) {
        cache[0] = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        if (i == 0) {           /* already set to 1 */
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t c = binomial(i - 1, 2 * k);           /* binomial(n-1,2k) */
            seq_t v = cache[i - 2 * k - 1];             /* a(n-2k-1) */
            sum += c * v;
        }
        cache[i] = 2 * sum;
    }

    /* update the next index to compute */
    if (n + 1 > next) {
        next = n + 1;
    }

    return cache[n];
}
