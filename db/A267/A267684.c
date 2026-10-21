/*
 * A267684  Binary representation of the n-th iteration of the "Rule 203"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*100^n - 9*10^n - 1)/9 for n > 1 a(0) = 1, a(1) = 100
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)100;

    /* compute 10^n and 10^(2n) (== 100^n) */
    const seq_t ten_pow   = pow10(n);
    const seq_t hundred_pow = pow10(2 * n); /* 10^(2n) = 100^n */

    /* numerator = 10 * 100^n - 9 * 10^n - 1 */
    const seq_t numer = (seq_t)10 * hundred_pow
                      - (seq_t)9 * ten_pow
                      - (seq_t)1;

    /* denominator is 9, division is exact */
    return numer / (seq_t)9;
}
