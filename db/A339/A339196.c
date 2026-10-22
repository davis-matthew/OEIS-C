/*
 * A339196  Number of (undirected) cycles on the n X 2 king graph.
 * https://oeis.org/A339196
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*2^n - 9*n - 7
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the closed‑form of the sequence with generating function */
/* G(x) = x^2*(7+2x)/((1-x)^2*(1-2x)) and recurrence */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 2; for smaller n return 0. */
    if (n < 2)
        return 0;

    /* Compute 8 * 2^n */
    seq_t pow2n = pow2(n);
    seq_t val   = pow2n * ((seq_t)8);

    /* Subtract 9*n + 7 */
    seq_t sub = (seq_t)(9 * n) + (seq_t)7;

    return val - sub;
}
