/*
 * A348858  G.f. A(x) satisfies: A(x) = 1 / ((1 - x) * (1 - x * A(3*x))).
 * https://oeis.org/A348858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} 3^k * a(k) * a(n-1-k) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

/* Memoized calculation of A348858 */
seq_t a(idx_t n) {
    /* Storage for computed values and powers of 3 */
    static seq_t *a_vals  = NULL;
    static size_t a_cap   = 0;
    static idx_t  a_len   = 0;

    static seq_t *pow3    = NULL;
    static size_t pow3_cap = 0;
    static idx_t  pow3_len = 0;

    /* Make sure we have room up to n */
    if ((size_t)n + 1 > a_cap) {
        size_t need = (size_t)n + 1;
        ensure_capacity(need, &a_cap, (void **)&a_vals, sizeof(seq_t));
        ensure_capacity(need, &pow3_cap, (void **)&pow3, sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) {
            a_vals[0] = (seq_t)1;
            pow3[0]   = (seq_t)1;
            a_len = 1;
            pow3_len = 1;
            continue;
        }

        /* 3^i */
        pow3[i] = pow3[i - 1] * (seq_t)3;
        pow3_len = i + 1;

        /* Convolution sum */
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = pow3[k];
            term = term * a_vals[k];
            term = term * a_vals[i - 1 - k];
            sum += term;
        }
        a_vals[i] = 1 + sum;
        ++a_len;
    }

    return a_vals[n];
}
