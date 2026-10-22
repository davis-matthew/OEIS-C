/*
 * A320667  First differences of A066194.
 * https://oeis.org/A320667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/6)*(-3 + (-1)^v2(n) +
 *            2^(v2(n)+3))*(-1)^(popcount(n)+1)
 * Range:     i8 n <= 31, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the first differences of A066194 (A320667). */
seq_t a(idx_t n) {
    /* Compute v2(n), the exponent of the largest power of 2 dividing n. */
    idx_t tmp = n;
    int v2 = 0;
    while ((tmp & 1) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* Compute popcount(n). */
    int pop = 0;
    tmp = n;
    while (tmp) {
        pop += tmp & 1;
        tmp >>= 1;
    }

    /* 2^(v2+3) using the provided util. */
    seq_t pow_val = pow2((idx_t)(v2 + 3));

    /* (-3 + (-1)^v2 + 2^(v2+3)) */
    seq_t t = - (seq_t)3 + ((v2 & 1) ? -(seq_t)1 : (seq_t)1) + pow_val;

    /* Divide by 6 (t is guaranteed divisible by 6). */
    seq_t res = t / (seq_t)6;

    /* Multiply by (-1)^(pop+1) => sign = +1 if pop odd, -1 if pop even. */
    if ((pop & 1) == 0) {
        res = -res;
    }

    return res;
}
