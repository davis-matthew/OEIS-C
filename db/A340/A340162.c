/*
 * A340162  a(1) = 7; thereafter a(n) is the smallest number k with exactly
 *          three 1-bits, not already in the sequence, for which k*a(n - 1)
 *          has exactly three 1-bits (A014311).
 * https://oeis.org/A340162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*2^(n/2) if n is even; a(n) = 19*2^((n+1)/4 - 1) if n =
 *            4k-1 (k >= 1); a(n) = 37*2^((n-1)/4 - 1) if n = 4k+1 (k >= 1);
 *            a(1) = 7
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 7 */
    if (n == 1)
        return (seq_t)7;

    /* Even indices: a(2k) = 7 * 2^k */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                 /* k = n/2 */
        return pow2(k) * (seq_t)7;         /* 7 * 2^k */
    }

    /* Odd indices */
    /* Determine if n ≡ 3 (mod 4) => 4k-1 case */
    if ((n + 1) % 4 == 0) {
        idx_t k = (n + 1) / 4;             /* k = (n+1)/4 */
        return pow2(k - 1) * (seq_t)19;     /* 19 * 2^(k-1) */
    }

    /* Remaining odd case: n ≡ 1 (mod 4) => 4k+1 case */
    idx_t k = (n - 1) / 4;                 /* k = (n-1)/4 */
    return pow2(k - 1) * (seq_t)37;         /* 37 * 2^(k-1) */
}
