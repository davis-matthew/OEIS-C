/*
 * A258831  Expansion of (psi(-x^3) * f(-x, x^2))^2 in powers of x where
 *          psi(), f(,) are Ramanujan theta functions.
 * https://oeis.org/A258831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the periodic sequence c of period
 *            12: c = [-2, 2, 0, 0, -2, -4, -2, 0, 0, 2, -2, -4]. a(0) = 1,
 *            and for n ≥ 1 a[n] = 1/n * Σ_{k=1..n} a[n-k] * Σ_{d|k} d *
 *            c[d-1]
 * Range:     i8 n <= 9, i16 n <= 186, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const long long c_arr[12] = { -2,  2,  0,  0, -2, -4, -2,  0,  0,  2, -2, -4 };

static seq_t *a_cache      = NULL;   /* a[n] values */
static seq_t *dsum_cache   = NULL;   /* dSum[k] values */
static idx_t  a_cap        = 0;       /* capacity of the caches */

/* Ensure caches are large enough for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (a_cap <= n) {
        idx_t new_cap = a_cap ? a_cap << 1 : 256;
        while (new_cap <= (idx_t)n) new_cap <<= 1;
        a_cache    = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        dsum_cache = (seq_t *)realloc(dsum_cache, new_cap * sizeof(seq_t));
        if (a_cache && dsum_cache) {
            /* Zero-initialize new space */
            for (idx_t i = a_cap; i < new_cap; ++i) {
                a_cache[i]    = 0;
                dsum_cache[i] = 0;
            }
            a_cap = new_cap;
        }
    }
}

/* Compute dSum[k] = Σ_{d|k} d * c[d-1] for all k up to n.
   dSum[0] is unused and left as 0. */
static void compute_dsum_up_to(idx_t n)
{
    static idx_t dsum_max = 0;
    if (dsum_max >= n) return;

    for (idx_t k = dsum_max + 1; k <= n; ++k) {
        seq_t s = 0;
        idx_t sq = (idx_t)(k * k > (idx_t)0 ? (k * k) : 0);
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;
                idx_t idx1 = (d1 - 1) % 12;
                idx_t idx2 = (d2 - 1) % 12;
                s += (seq_t)d1 * (seq_t)c_arr[idx1];
                if (d2 != d1) s += (seq_t)d2 * (seq_t)c_arr[idx2];
            }
        }
        dsum_cache[k] = s;
    }
    dsum_max = n;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    ensure_capacity(n);
    compute_dsum_up_to(n);

    /* Compute a[0] if not already */
    if (a_cache[0] == 0) a_cache[0] = (seq_t)1;

    /* Compute a[1..n] if required */
    static idx_t a_max = 0;
    if (a_max <= n) {
        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += dsum_cache[k] * a_cache[i - k];
            }
            a_cache[i] = sum / (seq_t)i;  /* exact division guaranteed */
        }
        a_max = n;
    }

    return a_cache[n];
}
