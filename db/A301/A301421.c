/*
 * A301421  Sums of positive coefficients of generalized Chebyshev polynomials
 *          of the first kind, for a family of 6 data.
 * https://oeis.org/A301421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            7*a(n-1)+9*a(n-2)+5*a(n-3)-5*a(n-4)-9*a(n-5)-5*a(n-6)-a(n-7)
 *            with a(1)=1, a(2)=6, a(3)=46, a(4)=371, a(5)=3026, a(6)=24707,
 *            a(7)=201748
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed a(n) values, 0‑based */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t cache_len = 0;      /* number of terms currently stored (a(1)…a(cache_len)) */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* make sure the buffer is large enough */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)      val = (seq_t)1;
        else if (i == 2) val = (seq_t)6;
        else if (i == 3) val = (seq_t)46;
        else if (i == 4) val = (seq_t)371;
        else if (i == 5) val = (seq_t)3026;
        else if (i == 6) val = (seq_t)24707;
        else if (i == 7) val = (seq_t)201748;
        else {
            /* i >= 8, compute via recurrence */
            val = 7 * cache[(size_t)(i - 2)];
            val += 9 * cache[(size_t)(i - 3)];
            val += 5 * cache[(size_t)(i - 4)];
            val -= 5 * cache[(size_t)(i - 5)];
            val -= 9 * cache[(size_t)(i - 6)];
            val -= 5 * cache[(size_t)(i - 7)];
            val -=  cache[(size_t)(i - 8)];
        }
        cache[(size_t)(i - 1)] = val;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
