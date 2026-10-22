/*
 * A307365  G.f. A(x) satisfies: A(x) = x*exp(A(-x) + A(-x^2)/2 + A(-x^3)/3 +
 *          A(-x^4)/4 + ...).
 * https://oeis.org/A307365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1. For n >= 1: a(n+1) = (1/n) * sum_{k=1..n}
 *            (sum_{d|k} (-1)^d * d * a(d)) * a(n-k+1)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 31, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* array of a values */
static seq_t *w_cache = NULL;   /* array of w values: w[k] = sum_{d|k} (-1)^d * d * a(d) */
static size_t a_cap = 0;        /* allocated capacity of a_cache */
static size_t w_cap = 0;        /* allocated capacity of w_cache */
static size_t a_len = 0;        /* number of computed terms in a_cache (a_len-1 is max index) */

static void
ensure_capacity(idx_t n, seq_t **cache, size_t *cap)
{
    if ((size_t)(n + 1) > *cap) {
        size_t new_cap = (*cap == 0) ? (n + 1 + 10) : (*cap * 2);
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cap   = new_cap;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Lazy initialization */
    if (a_cache == NULL) {
        size_t init_cap = (n + 1 > 2) ? (n + 1) : 2;
        a_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        w_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        a_cap   = w_cap = init_cap;
        a_len   = 2;            /* a[0] and a[1] are known */
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;

        /* compute w[1] */
        w_cache[0] = (seq_t)0;
        w_cache[1] = ((seq_t)-1) * (seq_t)1 * a_cache[1];
    }

    /* Ensure we have enough space for the requested n */
    ensure_capacity(n, &a_cache, &a_cap);
    ensure_capacity(n, &w_cache, &w_cap);

    /* Compute missing terms up to n */
    while (a_len <= (size_t)n) {
        idx_t d = (idx_t)(a_len - 1);   /* last computed index */

        /* Compute w[d] = sum_{t|d} (-1)^t * t * a(t) */
        seq_t wd = (seq_t)0;
        for (idx_t m = 1; m * m <= d; ++m) {
            if (d % m == 0) {
                idx_t d1 = m;
                seq_t sign1 = (d1 & 1) ? (seq_t)-1 : (seq_t)1;
                wd += sign1 * (seq_t)d1 * a_cache[d1];

                idx_t d2 = d / m;
                if (d2 != d1) {
                    seq_t sign2 = (d2 & 1) ? (seq_t)-1 : (seq_t)1;
                    wd += sign2 * (seq_t)d2 * a_cache[d2];
                }
            }
        }
        w_cache[d] = wd;

        /* Compute a[d+1] = (1/d) * sum_{k=1..d} w[k] * a[d-k+1] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= d; ++k) {
            sum += w_cache[k] * a_cache[d - k + 1];
        }
        a_cache[d + 1] = sum / (seq_t)d;
        ++a_len;
    }

    return a_cache[n];
}
