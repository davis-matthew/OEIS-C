/*
 * A346739  Expansion of e.g.f.: exp(exp(x) - 4*x - 1).
 * https://oeis.org/A346739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (-4)^(n-k) * Bell(k)
 *            Recurrence used: a(0) = 1, a(n) = -4 * a(n-1) + Sum_{k=0}^{n-1}
 *            binomial(n-1,k) * a(k) (n≥1)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 29, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* memoized values */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t   computed_n = (idx_t)-1;/* highest index computed so far */

/* compute a(n) using dynamic programming + recurrence */
seq_t a(idx_t n)
{
    /* allocate / enlarge cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1 > 1 ? n + 1 : 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;                 /* a(0) = 1 */
        computed_n = 0;
    } else if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* base case */
    if (n == 0)
        return cache[0];

    /* compute values up to n if needed */
    while (computed_n < n) {
        idx_t m = computed_n;            /* m = new_n - 1 */
        seq_t sum = 0;
        seq_t binom = 1;                  /* C(m,0) */

        /* sum_{k=0}^{m} C(m,k) * a(k) */
        for (idx_t k = 0; k <= m; ++k) {
            sum += binom * cache[k];
            if (k < m) {
                binom = binom * (m - k) / (k + 1); /* next binomial coefficient */
            }
        }

        seq_t val = -4 * cache[computed_n] + sum; /* recurrence */
        ++computed_n;
        cache[computed_n] = val;
    }

    return cache[n];
}
