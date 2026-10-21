/*
 * A292055  Wiener index of the n-Mycielski graph.
 * https://oeis.org/A292055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (216 - 27*2^(n + 3) - 56*3^n + 81*4^n)/144 (n > 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1 the sequence defines a(1) = 0. */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* The first term is explicitly defined. */
    if (n == 1)
        return (seq_t)0;

    /* Compute the needed powers of 2, 3 and 4. */
    seq_t pow2 = pow_uint(2, n + 3);   /* 2^(n+3) */
    seq_t pow3 = pow_uint(3, n);       /* 3^n     */
    seq_t pow4 = pow_uint(4, n);       /* 4^n     */

    /* Evaluate the numerator of the closed‑form expression. */
    seq_t numerator = (seq_t)216
                     - (seq_t)27 * pow2
                     - (seq_t)56 * pow3
                     + (seq_t)81 * pow4;

    /* Divide by the constant 144. */
    return numerator / (seq_t)144;
}
