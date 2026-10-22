/*
 * A356815  Expansion of e.g.f. exp(-x * (exp(2*x) + 1)).
 * https://oeis.org/A356815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (2*k-1)^(n-k) * binomial(n,k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(0) = 1, a(1) = -2, a(2) = 0, ... as defined in OEIS A356815. */
/* Memoised implementation that works for arbitrary precision seq_t and idx_t. */

#include <stdlib.h>
#include <stdint.h>

/* Internal static storage */
static seq_t *cache = NULL;          /* memoised sequence values */
static size_t cache_size = 0;        /* number of elements currently stored (0..cache_size-1) */
static size_t cache_cap = 0;         /* capacity of cache array */

static seq_t *pow_arr = NULL;        /* array of (2*k-1)^(n-k) for the current n */
static size_t pow_size = 0;          /* number of elements stored in pow_arr (0..pow_size-1) */
static size_t pow_cap = 0;           /* capacity of pow_arr */

/* Ensure the memoised array contains a(n) for all n up to `need` */
static void ensure_cache(idx_t need)
{
    if (need < cache_size)
        return;

    /* Expand cache array if necessary */
    size_t new_cap = need + 1;
    if (new_cap > cache_cap) {
        cache_cap = new_cap + new_cap / 2 + 16;   /* grow by 1.5x + 16 */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – terminate */
            abort();
        }
        cache = tmp;
    }

    /* Expand pow array if necessary */
    if (new_cap > pow_cap) {
        pow_cap = new_cap + new_cap / 2 + 16;
        seq_t *tmp = (seq_t *)realloc(pow_arr, pow_cap * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        pow_arr = tmp;
    }

    /* Compute missing terms */
    for (idx_t n = cache_size; n <= need; ++n) {
        if (n == 0) {
            /* a(0) = 1, pow[0] = 1 */
            cache[0] = (seq_t)1;
            pow_arr[0] = (seq_t)1;
        } else {
            /* Update pow array: multiply existing values by (2*k-1) */
            for (idx_t k = 0; k < n; ++k) {
                long long mult = 2LL * (long long)k - 1;  /* may be negative for k = 0 */
                pow_arr[k] = pow_arr[k] * mult;
            }
            /* New element pow[n] = (2*n-1)^(n-n) = 1 */
            pow_arr[n] = (seq_t)1;

            /* Compute a(n) using binomial coefficients iteratively */
            seq_t sum = (seq_t)0;
            seq_t comb = (seq_t)1;   /* C(n, 0) */
            for (idx_t k = 0; k <= n; ++k) {
                seq_t term = comb * pow_arr[k];
                if (k & 1) {
                    term = -term;   /* multiply by (-1)^k */
                }
                sum += term;
                if (k < n) {
                    /* Prepare comb for next k: C(n, k+1) = C(n, k) * (n-k) / (k+1) */
                    comb = comb * ((idx_t)(n - k));
                    comb = comb / ((idx_t)(k + 1));
                }
            }
            cache[n] = sum;
        }
    }

    cache_size = need + 1;
    pow_size = need + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
