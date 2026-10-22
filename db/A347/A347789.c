/*
 * A347789  a(n) is the number of times that only 2 pegs have disks on them
 *          during the optimal solution to a Towers of Hanoi problem with n
 *          disks.
 * https://oeis.org/A347789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + (n mod 2)) * 2^floor(n/2) - 4, with offset 1
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1; a guard is provided for safety. */
    if (n == 0)
        return 0;

    idx_t half = n / 2;                    /* floor(n/2) */
    seq_t p = pow2(half);                  /* 2^floor(n/2) */
    seq_t mult = (seq_t)3 + (seq_t)(n & 1); /* 3 + (n % 2) */

    seq_t res = mult * p;
    res -= (seq_t)4;                       /* subtract 4 */
    return res;
}
