/*
 * A352383  G.f. A(x) satisfies: A(x) = (1 + 2*x*A(x)^3)^2 / (1 + x*A(x)^3).
 * https://oeis.org/A352383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A352383: a(0)=1, and for n≥1
   a(n) = 4 * coeff(A^3, n-1) + 4 * coeff(A^6, n-2) - coeff(A^4, n-1),
   where coeff(A^k, m) is the coefficient of x^m in A(x)^k. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values and intermediate
       power series coefficients up to the current maximum index. */
    static seq_t *a_vals   = NULL; /* a[0..] */
    static seq_t *p2_vals  = NULL; /* coeff(A^2, *) */
    static seq_t *p3_vals  = NULL; /* coeff(A^3, *) */
    static seq_t *p4_vals  = NULL; /* coeff(A^4, *) */
    static seq_t *p6_vals  = NULL; /* coeff(A^6, *) */
    static idx_t cache_len = 0;    /* number of filled entries (≥1 after init) */

    /* extend cache if the requested index exceeds the current size */
    if (n >= cache_len) {
        idx_t new_cap = n + 1;

        a_vals   = (seq_t*)realloc(a_vals,   new_cap * sizeof(seq_t));
        p2_vals  = (seq_t*)realloc(p2_vals,  new_cap * sizeof(seq_t));
        p3_vals  = (seq_t*)realloc(p3_vals,  new_cap * sizeof(seq_t));
        p4_vals  = (seq_t*)realloc(p4_vals,  new_cap * sizeof(seq_t));
        p6_vals  = (seq_t*)realloc(p6_vals,  new_cap * sizeof(seq_t));

        if (cache_len == 0) {            /* first-time initialisation */
            a_vals[0]   = 1;
            p2_vals[0]  = 1;              /* A^2 coefficient at x^0 */
            p3_vals[0]  = 1;              /* A^3 coefficient at x^0 */
            p4_vals[0]  = 1;              /* A^4 coefficient at x^0 */
            p6_vals[0]  = 1;              /* A^6 coefficient at x^0 */
            cache_len   = 1;              /* index 0 is ready */
        }

        /* compute new coefficients up to requested index n */
        for (idx_t m = cache_len; m <= n; ++m) {
            /* ---------- compute a[m] ----------
             * a_m = 4 * p3[m-1] + 4 * p6[m-2] - p4[m-1]
             * p6[m-2] is zero when m < 2. */
            seq_t term = 4 * p3_vals[m - 1] - p4_vals[m - 1];
            if (m >= 2) term += 4 * p6_vals[m - 2];
            a_vals[m] = term;

            /* ---------- update p2[m] = Σ a_i * a_{m-i} ----------
             */
            seq_t sum2 = 0;
            for (idx_t i = 0; i <= m; ++i)
                sum2 += a_vals[i] * a_vals[m - i];
            p2_vals[m] = sum2;

            /* ---------- update p3[m] = Σ a_i * p2[m-i] ----------
             */
            seq_t sum3 = 0;
            for (idx_t i = 0; i <= m; ++i)
                sum3 += a_vals[i] * p2_vals[m - i];
            p3_vals[m] = sum3;

            /* ---------- update p4[m] = Σ a_i * p3[m-i] ----------
             */
            seq_t sum4 = 0;
            for (idx_t i = 0; i <= m; ++i)
                sum4 += a_vals[i] * p3_vals[m - i];
            p4_vals[m] = sum4;

            /* ---------- update p6[m] = Σ p3[i] * p3[m-i] ----------
             */
            seq_t sum6 = 0;
            for (idx_t i = 0; i <= m; ++i)
                sum6 += p3_vals[i] * p3_vals[m - i];
            p6_vals[m] = sum6;
        }

        cache_len = new_cap;   /* all entries up to n are ready */
    }

    return a_vals[n];
}
