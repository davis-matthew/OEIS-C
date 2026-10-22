/*
 * A321384  a(1) = 1; a(n+1) = -Sum_{d|n} d*a(d).
 * https://oeis.org/A321384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n+1) = -∑_{d|n} d * a(d) (OEIS A321384)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;            /* 1‑based array holding a(n) */
static idx_t  alloc_len = 0;           /* allocated length of cache (index+1) */
static idx_t  last_computed = 0;       /* highest index that has been filled */

/* Ensure cache can address indices up to n (including n). */
static void ensure_cache(idx_t n)
{
    if (alloc_len >= n + 1) return;
    idx_t new_len = n + 1;                     /* +1 because we ignore index 0 */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                        /* allocation failure */
    cache = tmp;
    alloc_len = new_len;
}

/* Public sequence function.  Works for repeated or out‑of‑order calls. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n<1, safe */

    ensure_cache(n);

    /* initialise a(1) if necessary */
    if (last_computed < 1) {
        cache[1] = 1;
        last_computed = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;                     /* already set but keep for clarity */
            continue;
        }
        idx_t m = i - 1;                       /* we need divisors of i-1 */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                seq_t prod = (seq_t)d * cache[d];
                sum += prod;
                idx_t d2 = m / d;
                if (d2 != d) {
                    prod = (seq_t)d2 * cache[d2];
                    sum += prod;
                }
            }
        }
        cache[i] = -sum;                       /* a(i) = -∑_{d|i-1} d*a(d) */
    }

    last_computed = n;
    return cache[n];
}
