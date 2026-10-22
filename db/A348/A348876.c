/*
 * A348876  G.f. A(x) satisfies: A(x) = 1 / (1 - x - x * A(3*x)).
 * https://oeis.org/A348876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A348876: a(0)=1;
   a(n)=a(n-1)+∑_{k=0}^{n-1} 3^k * a(k) * a(n-k-1)  (n≥1) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static storage for the sequence, powers of 3 and capacity/length */
    static seq_t *seq      = NULL;
    static seq_t *pow3     = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;   /* number of terms actually computed */

    /* ensure enough capacity for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ?: 1;
        while (new_cap < needed) new_cap <<= 1;
        seq   = (seq_t *)realloc(seq,   new_cap * sizeof(seq_t));
        pow3  = (seq_t *)realloc(pow3,  new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* initialise the base case once */
    if (seq_len == 0) {
        seq[0]  = (seq_t)1;
        pow3[0] = (seq_t)1;
        seq_len = 1;
    }

    /* compute missing terms up to n */
    for (size_t i = seq_len; i <= n; ++i) {
        if (i > 0) {
            /* next power of 3 */
            pow3[i] = pow3[i - 1] * (seq_t)3;
        }

        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < i; ++k) {
            seq_t term = pow3[k] * seq[k];
            term *= seq[i - 1 - k];
            sum += term;
        }

        seq[i] = seq[i - 1] + sum;
    }
    seq_len = (size_t)n + 1;

    return seq[n];
}
