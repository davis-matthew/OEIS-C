/*
 * A317060  a(n) is the number of time-dependent assembly trees satisfying the
 *          edge gluing rule for a cycle on n vertices.
 * https://oeis.org/A317060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=1..floor(n/2)} ( binomial(n-j, n-2j) +
 *            binomial(n-j-1, n-2j) ) * a(n-j) a(1) = a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* 1‑based cache of computed terms */
static idx_t  cache_size = 0;    /* allocated size of the cache */
static idx_t  max_comp   = 0;    /* largest n for which cache[n] is known */

/* Return the nth term of OEIS A317060 (1‑based index) */
seq_t a(idx_t n)
{
    /* n < 1 is undefined in OEIS; return 0 as a safe default */
    if (n < 1)
        return (seq_t)0;

    /* Resize the cache if necessary */
    if (n >= cache_size) {
        idx_t oldsize = cache_size;
        idx_t newsize = n + 1;                     /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp)                                  /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        /* Initialize new entries to 0 */
        for (idx_t i = oldsize; i < newsize; ++i)
            cache[i] = (seq_t)0;
        cache_size = newsize;
    }

    /* Compute all terms up to n that are still missing */
    for (idx_t m = max_comp + 1; m <= n; ++m) {
        if (m == 1 || m == 2) {
            cache[m] = (seq_t)1;              /* base cases */
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t maxj = m / 2;
        for (idx_t j = 1; j <= maxj; ++j) {
            seq_t t1 = binomial(m - j, m - 2 * j);
            seq_t t2 = binomial(m - j - 1, m - 2 * j);
            sum += (t1 + t2) * cache[m - j];
        }
        cache[m] = sum;
    }
    if (n > max_comp)
        max_comp = n;

    return cache[n];
}
