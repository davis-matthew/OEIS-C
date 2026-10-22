/*
 * A342085  Number of decreasing chains of distinct superior divisors starting
 *          with n.
 * https://oeis.org/A342085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d|n, d < n, d² ≥ n} a(d) a(1) = 1. Count of
 *            decreasing chains of distinct superior divisors starting with n
 * Range:     u8 n <= 287, u16 n <= 9604, u32 n <= 9604, u64 n <= 9604,
 *            u128 n <= 9604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t _computed = 0;
static seq_t * _cache = NULL;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* offset starts at 1 */

    /* Grow cache if necessary.  Index 0 is unused, indexes 1..n are stored. */
    if (_computed < n) {
        size_t new_cap = (size_t)n + 1;      /* capacity for indices 0..n */
        if (_cache == NULL) {
            _cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (_cache == NULL) return 0;    /* allocation failed */
        } else if (new_cap > (size_t)_computed + 1) {
            seq_t *tmp = (seq_t *)realloc(_cache,
                                          new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;      /* allocation failed */
            _cache = tmp;
        }

        /* Compute a(k) for k = _computed+1 .. n */
        for (idx_t k = _computed + 1; k <= n; ++k) {
            seq_t sum = 1;                 /* chain consisting only of k */

            /* Enumerate proper divisors d >= 2 up to sqrt(k) */
            for (idx_t d = 2; d <= k / d; ++d) {   /* avoid overflow */
                if (k % d == 0) {
                    idx_t e = k / d;        /* complementary divisor, e < k */
                    sum += _cache[e];
                }
            }
            _cache[k] = sum;
        }
        _computed = n;
    }
    return _cache[n];
}
