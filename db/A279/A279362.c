/*
 * A279362  Expansion of psi(x)^2 * chi(-x^5) in powers of x where psi(),
 *          chi() are Ramanujan theta functions.
 * https://oeis.org/A279362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑10 sequence c = [ 2,
 *            -2, 2, -2, 1, -2, 2, -2, 2, -2 ]. Thus A(x) = ∏_{k≥1} 1/(1 - c_k
 *            x^k) (with c_k = c[(k-1) mod 10]), and a(0)=1. The recurrence is
 *            a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d*c_d ) * a(n-k)
 * Range:     i8 n <= 35, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const int period10[10] = {  2, -2,  2, -2,  1, -2,  2, -2,  2, -2 };

/* Cached values of a(n) and the auxiliary b(k)=Σ_{d|k} d*c_d. */
static seq_t *cache_a = NULL;      /* a[0..cap-1]   */
static seq_t *cache_b = NULL;      /* b[0..cap-1]   */
static idx_t  cache_cap = 0;       /* allocated size */
static idx_t  cache_max = (idx_t)-1;/* largest index computed, -1 if none */

/* Ensure cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Allocate initial storage if needed */
    if (cache_a == NULL) {
        cache_cap = 16;
        cache_a = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        cache_b = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache_a || !cache_b) {
            /* Allocation failure – abort program. */
            abort();
        }
        cache_a[0] = (seq_t)1;   /* a(0) = 1 */
        cache_b[0] = (seq_t)0;   /* b(0) unused but set to 0 */
        cache_max = 0;
    }

    if (n <= cache_max) return;     /* already computed */

    /* Grow capacity until it can hold index n */
    while (cache_cap <= n) {
        cache_cap *= 2;
    }
    seq_t *tmp_a = (seq_t *)realloc(cache_a, (size_t)cache_cap * sizeof(seq_t));
    seq_t *tmp_b = (seq_t *)realloc(cache_b, (size_t)cache_cap * sizeof(seq_t));
    if (!tmp_a || !tmp_b) {
        abort();
    }
    cache_a = tmp_a;
    cache_b = tmp_b;

    /* Compute missing terms for k = cache_max+1 .. n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* ---- Compute b_k = Σ_{d|k} d * c_d ---- */
        int64_t b_k = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int cd = period10[(d - 1) % 10];
                b_k += (int64_t)d * cd;
                idx_t d2 = k / d;
                if (d2 != d) {
                    int cd2 = period10[(d2 - 1) % 10];
                    b_k += (int64_t)d2 * cd2;
                }
            }
        }
        cache_b[k] = (seq_t)b_k;

        /* ---- Compute a_k = (1/k) * Σ_{m=1..k} b_m * a_{k-m} ---- */
        int64_t sum = 0;
        for (idx_t m = 1; m <= k; ++m) {
            sum += (int64_t)cache_b[m] * (int64_t)cache_a[k - m];
        }
        cache_a[k] = (seq_t)(sum / (int64_t)k);
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* a(0)=1 */

    ensure_cache(n);
    return cache_a[n];
}
