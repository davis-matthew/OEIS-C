/*
 * A381210  Expansion of e.g.f. 1/(1 - sinh(x))^3.
 * https://oeis.org/A381210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the exponential generating function
 *            * 1/(1 - sinh(x))^3. * The algorithm expands the e.g.f. as a
 *            series of egfs, using the identities * (1 - s)^-1 = Σ_{k≥0} s^k,
 *            * (1 - s)^-3 = ( (1 - s)^-1 )^3, * where s(x)=sinh(x). The
 *            coefficients of 1/(1 - s) (named g_n) satisfy *...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t cached_len = 0;
static seq_t *a_cache = NULL;

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < cached_len)
        return a_cache[n];

    /* Allocate new arrays up to required size. */
    size_t sz = (size_t)n + 1;
    seq_t *g = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *h = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *a  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!g || !h || !a) {
        /* Allocation failed – abort. */
        abort();
    }

    /* 1. Compute g[n] = coeff of 1/(1 - sinh(x)). */
    g[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t bin = (seq_t)1;      /* binom(m,0) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            bin = bin * (seq_t)(m - k + 1) / (seq_t)k; /* binom(m,k) */
            if ((k & 1) == 1)                       /* odd k only */
                sum += bin * g[m - k];
        }
        g[m] = sum;
    }

    /* 2. Compute h = g * g (e.g.f. product). */
    h[0] = g[0] * g[0];
    for (idx_t m = 1; m <= n; ++m) {
        seq_t bin = (seq_t)1;          /* binom(m,0) */
        seq_t sum = (seq_t)0;
        /* i = 0 term */
        sum += bin * g[0] * g[m];
        for (idx_t i = 1; i <= m; ++i) {
            bin = bin * (seq_t)(m - i + 1) / (seq_t)i; /* binom(m,i) */
            sum += bin * g[i] * g[m - i];
        }
        h[m] = sum;
    }

    /* 3. Compute f = h * g (e.g.f. product), which is the desired sequence. */
    a[0] = h[0] * g[0];
    for (idx_t m = 1; m <= n; ++m) {
        seq_t bin = (seq_t)1;          /* binom(m,0) */
        seq_t sum = (seq_t)0;
        /* i = 0 term */
        sum += bin * h[0] * g[m];
        for (idx_t i = 1; i <= m; ++i) {
            bin = bin * (seq_t)(m - i + 1) / (seq_t)i; /* binom(m,i) */
            sum += bin * h[i] * g[m - i];
        }
        a[m] = sum;
    }

    /* 4. Replace cached array with new result and free temporary arrays. */
    free(a_cache);
    a_cache = a;
    cached_len = sz;
    free(g);
    free(h);

    return a_cache[n];
}
