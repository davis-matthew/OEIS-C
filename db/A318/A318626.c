/*
 * A318626  Number of 5-member subsets of [5*n] whose elements sum to a
 *          multiple of n.
 * https://oeis.org/A318626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3125*n^4 - 6250*n^3 + 4375*n^2 - 1250*n + 216 + 96*b(n))
 *            / 120 where b(n) = 4 if n % 5 == 0 else -1, for n ≥ 1; a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 113, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use 128‑bit signed arithmetic to avoid intermediate overflow. */
    __int128 sn = n;                 /* promote index to 128‑bit */
    __int128 n2 = sn * sn;
    __int128 n3 = n2 * sn;
    __int128 n4 = n3 * sn;

    __int128 num = 3125 * n4
                 - 6250 * n3
                 + 4375 * n2
                 - 1250 * sn
                 + 216;

    /* 5‑periodic correction: b(n) = 4 when n % 5 == 0, else -1. */
    int b = (n % 5 == 0) ? 4 : -1;
    num += 96 * (__int128)b;

    /* The division is exact. */
    num /= 120;

    return (seq_t)num;
}
