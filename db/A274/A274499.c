/*
 * A274499  Sum of the degrees of asymmetry of all ternary words of length n.
 * https://oeis.org/A274499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(2n - 1 + (-1)^n)*3^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A274499, the sum of degrees of asymmetry of ternary words. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the parity‑dependent factor: 2n for even n, 2(n-1) for odd n. */
    seq_t sn = (seq_t)n;
    seq_t factor = (n & 1U) ? (seq_t)(2 * (n - 1)) : (seq_t)(2 * n);

    /* Compute 3^n with binary exponentiation. */
    seq_t power = 1;
    seq_t base   = (seq_t)3;
    idx_t e = n;
    while (e > 0) {
        if (e & 1U) {
            power *= base;
        }
        e >>= 1U;
        if (e) {
            base *= base;
        }
    }

    /* Final computation: (factor * 3^n) / 6.  Division is exact. */
    seq_t result = (factor * power) / (seq_t)6;
    return result;
}
