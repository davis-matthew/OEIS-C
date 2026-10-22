/*
 * A326555  a(n) = (2^n + 3^n)^n for n>= 0.
 * https://oeis.org/A326555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n + 3^n)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow2, pow3, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/seq_pow.h"

/* Computes a(n) = (2^n + 3^n)^n for n >= 0. */
seq_t a(idx_t n)
{
    /* Compute 2^n and 3^n as big integers. */
    seq_t two_n = pow2(n);
    seq_t three_n = pow3(n);

    /* Sum of these powers. */
    seq_t sum = two_n + three_n;

    /* Raise the sum to the n-th power. */
    return seq_pow(sum, n);
}
