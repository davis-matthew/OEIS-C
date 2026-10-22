/*
 * A302102  G.f. A(x) satisfies: A(x) = 1 + x * (x*A(x)^4)' / (x*A(x))'.
 * https://oeis.org/A302102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) satisfies A(x) = 1 + x * (x*A(x)^4)' / (x*A(x))' This
 *            implementation computes the coefficients a(n) of A(x) using the
 *            recurrence a(n) = n * Σ_{i=0}^{n-1} conv2[i] * conv2[n-1-i] -
 *            Σ_{i=1}^{n-1} a(i) * (n-i+1) * a(n-i) where conv2[k] = Σ_{i=0}^k
 *            a(i) * a(k-i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static storage for computed terms */
    static size_t     a_cap   = 0;   /* capacity of a_vals   */
    static size_t     conv2_cap = 0; /* capacity of conv2    */
    static idx_t      max_n   = 0;   /* highest n already computed */

    static seq_t *a_vals   = NULL; /* a[0..]                 */
    static seq_t *conv2    = NULL; /* conv2[0..]              */

    /* ---------- initialization (once) ---------- */
    if (a_vals == NULL) {
        a_cap = 1;
        a_vals = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = 1;             /* a(0) = 1            */

        conv2_cap = 1;
        conv2 = (seq_t*)malloc(conv2_cap * sizeof(seq_t));
        conv2[0] = 1;              /* conv2(0) = a0*a0 = 1 */

        max_n = 0;
    }

    /* ---------- grow arrays if needed ---------- */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        a_vals = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        for (size_t i = a_cap; i < new_cap; ++i) a_vals[i] = 0;
        a_cap = new_cap;
    }

    /* ---------- compute missing terms up to n ---------- */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* sum1 = Σ_{i=1}^{cur-1} a(i) * (cur-i+1) * a(cur-i) */
        seq_t sum1 = 0;
        for (idx_t i = 1; i < cur; ++i) {
            seq_t term = a_vals[(size_t)i];
            term *= (seq_t)(cur - i + 1);
            term *= a_vals[(size_t)(cur - i)];
            sum1 += term;
        }

        /* sum2 = Σ_{i=0}^{cur-1} conv2[i] * conv2[cur-1-i] */
        seq_t sum2 = 0;
        for (idx_t i = 0; i < cur; ++i) {
            sum2 += conv2[(size_t)i] * conv2[(size_t)(cur - 1 - i)];
        }

        /* a(cur) = cur * sum2 - sum1 */
        seq_t a_cur = (seq_t)cur * sum2 - sum1;
        a_vals[(size_t)cur] = a_cur;

        /* ---------- update conv2 for pairs involving a_cur ---------- */
        /* ensure conv2 capacity at least 2*cur+1 */
        size_t required_cap = (size_t)2 * cur + 1;
        if (conv2_cap < required_cap) {
            conv2 = (seq_t*)realloc(conv2,
                    required_cap * sizeof(seq_t));
            for (size_t i = conv2_cap; i < required_cap; ++i)
                conv2[i] = 0;
            conv2_cap = required_cap;
        }

        /* add contributions of ordered pairs (n, j) for j < cur */
        for (idx_t j = 0; j < cur; ++j) {
            conv2[(size_t)(cur + j)] += (seq_t)2 * a_cur * a_vals[(size_t)j];
        }
        /* add contribution of pair (cur, cur) once */
        conv2[(size_t)(2 * cur)] += a_cur * a_cur;
    }

    max_n = n;
    return a_vals[(size_t)n];
}
