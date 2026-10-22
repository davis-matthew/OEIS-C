/*
 * A351189  G.f. A(x) satisfies: A(x) = x^2 + x^3 * A(x/(1 + x)) / (1 + x).
 * https://oeis.org/A351189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 1; a(n) = Sum_{k=0..n-3} (-1)^k *
 *            C(n-3,k) * a(n-k-3) for n >= 3
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 24, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* allocated size of cache  */
static idx_t  computed_upto = 0;     /* maximum n for which a(n) is stored */

static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((idx_t)cache_cap <= n) {
        while (cache_cap <= (size_t)n)
            cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Returns a(n) for the OEIS sequence A351189. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;
    if (n == 2)
        return 1;

    /* make sure the cache can hold value n */
    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t m = computed_upto + 1; m <= n; ++m) {
        if (m == 0 || m == 1) {
            cache[m] = 0;
        } else if (m == 2) {
            cache[m] = 1;
        } else {
            idx_t limit = m - 3;
            seq_t C = 1;        /* C( limit , 0 ) */
            seq_t sign = 1;     /* (-1)^0 = 1   */
            seq_t sum  = 0;     /* accumulator */

            for (idx_t j = 0; j <= limit; ++j) {
                /* term = C * a(m - 3 - j) */
                seq_t term = C * cache[m - 3 - j];
                if (sign > 0)
                    sum += term;
                else
                    sum -= term;

                if (j < limit) {
                    /* Update C to C(limit, j+1) = C * (limit-j)/(j+1) */
                    seq_t num = limit - j;
                    seq_t den = j + 1;
                    C = (C * num) / den;

                    /* Toggle sign for next iteration */
                    sign = -sign;
                }
            }
            cache[m] = sum;
        }
    }

    computed_upto = n > computed_upto ? n : computed_upto;
    return cache[n];
}
