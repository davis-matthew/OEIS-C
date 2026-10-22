/*
 * A351186  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 + 5*x)) / (1 +
 *          5*x).
 * https://oeis.org/A351186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1, a(n)=∑_{k=0..n-2}
 *            binomial(n-2,k)*(-5)^k*a(n-k-2)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A351186).  The values are cached lazily and memoized. */
#include <stdlib.h>

/* static cache for computed values */
static seq_t *cache   = NULL;   /* array of values               */
static idx_t  cap    = 0;       /* allocated size                */
static idx_t  last   = 0;       /* last index that has been set  */

/* extend cache to at least (need+1) entries and initialise new slots */
static void
ensure_cache(idx_t need)
{
    if (need < cap) return;

    idx_t new_cap = need + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure – abort */
    cache = tmp;

    /* initialise the new slots to zero */
    for (idx_t i = cap; i < new_cap; ++i)
        cache[i] = 0;
    cap = new_cap;

    /* initialise base values if not already present */
    if (cap > 0) {
        cache[0] = 1;
        last = 0;
    }
    if (cap > 1) {
        cache[1] = 1;
        last = 1;
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n <= last)           /* already computed */
        return cache[n];

    /* compute values from last+1 up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        if (m == 0 || m == 1) {        /* safety, although handled in ensure_cache */
            cache[m] = 1;
            continue;
        }

        idx_t k_max = m - 2;
        seq_t sum   = 0;
        seq_t coeff = 1;                /* binom(m-2,0) * (-5)^0 */

        for (idx_t k = 0; k <= k_max; ++k) {
            sum += coeff * cache[m - k - 2];

            if (k < k_max) {
                /* coeff = coeff * (m-2-k) * (-5) / (k+1)  */
                coeff = coeff * (seq_t)(m - 2 - k);
                coeff = coeff * (seq_t)(-5);
                coeff = coeff / (seq_t)(k + 1);
            }
        }
        cache[m] = sum;
    }
    last = n;          /* all values up to n are now computed */
    return cache[n];
}
