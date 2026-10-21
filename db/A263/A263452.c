/*
 * A263452  Expansion of f(-q^3)^3 * psi(q^12) / f(-q) in powers of q where
 *          ps(), f() are Ramanujan theta functions.
 * https://oeis.org/A263452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Euler transform of the periodic sequence b(k) = [1, 1,
 *            -2, 1, 1, -2, 1, 1, -2, 1, 1, -1, 1, 1, -2, 1, 1, -2, 1, 1, -2,
 *            1, 1, -3] (period 24) The recurrence is a(0) = 1 a(n) = (1/n) *
 *            Σ_{k=1..n} ( Σ_{d|k} d*b(d) ) * a(n-k) for n ≥ 1. The array is
 *            memoised for arbitrary order of calls
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Periodic sequence b(k) of length 24 */
static const signed char period_b[24] = {
    1,  1, -2,  1,  1, -2,  1,  1, -2,  1,  1, -1,
    1,  1, -2,  1,  1, -2,  1,  1, -2,  1,  1, -3
};

seq_t a(idx_t n)
{
    /* Static state retained between calls */
    static seq_t *a_arr = NULL;          /* coefficients a(0..max_n) */
    static signed char *b_arr = NULL;    /* b(k) for k=1..max_n */
    static int32_t  *D_arr = NULL;       /* D(k) = Σ_{d|k} d*b(d) */
    static size_t   cap = 0;             /* capacity of the above arrays */
    static idx_t    max_n = (idx_t)-1;    /* highest n computed so far */

    /* Initialise on first call */
    if (max_n == (idx_t)-1) {
        cap = 1;
        a_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        b_arr = (signed char *)malloc(cap * sizeof(signed char));
        D_arr = (int32_t *)malloc(cap * sizeof(int32_t));
        if (!a_arr || !b_arr || !D_arr) return 0; /* allocation failure */

        a_arr[0] = (seq_t)1;   /* a(0) = 1 */
        b_arr[0] = 0;          /* unused */
        D_arr[0] = 0;
        max_n = 0;
    }

    /* Extend storage if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        b_arr = (signed char *)realloc(b_arr, new_cap * sizeof(signed char));
        D_arr = (int32_t *)realloc(D_arr, new_cap * sizeof(int32_t));
        if (!a_arr || !b_arr || !D_arr) return 0;           /* allocation failure */
        cap = new_cap;
    }

    /* Compute values up to n if not already done */
    if (n > max_n) {
        /* Precompute b(k) and D(k) for new indices */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* b(k) from periodic sequence (period 24) */
            int idx = (int)((k - 1) % 24);
            signed char b_val = period_b[idx];
            b_arr[k] = b_val;

            /* D(k) = Σ_{d|k} d*b(d) */
            int64_t sumB = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    sumB += (int64_t)d * (int64_t)b_arr[d];
                    idx_t e = k / d;
                    if (e != d)
                        sumB += (int64_t)e * (int64_t)b_arr[e];
                }
            }
            D_arr[k] = (int32_t)sumB;
        }

        /* Compute a(n) for each new index using the recurrence */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            __int128 sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                sum += (__int128)D_arr[k] * (__int128)a_arr[m - k];
            }
            a_arr[m] = (seq_t)(sum / m);   /* division is exact */
        }
        max_n = n;
    }

    return a_arr[n];
}
