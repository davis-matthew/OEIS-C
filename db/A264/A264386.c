/*
 * A264386  Gergonne's 27-card trick with three piles: finding a card after
 *          three dealings with pile information.
 * https://oeis.org/A264386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = (reversed((n)_3))_10 + 1
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce n modulo 27 (period of the sequence) */
    idx_t m = n % 27;

    /* ternary digits of m (3 digits with leading zeros) */
    idx_t d0 = m % 3;         /* least significant digit  */
    idx_t d1 = (m / 3) % 3;   /* middle digit            */
    idx_t d2 = (m / 9) % 3;   /* most significant digit  */

    /* reverse the digits: digit order is d0 d1 d2 -> d2 d1 d0 */
    seq_t rev = (seq_t)d0 * 9 + (seq_t)d1 * 3 + (seq_t)d2;

    /* add 1 as per the definition */
    return rev + (seq_t)1;
}
