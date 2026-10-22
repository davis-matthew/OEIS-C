/*
 * A400616  a(n) is the signed number of partitions of n into exactly two
 *          distinct part sizes, each occurring an odd number of times.
 * https://oeis.org/A400616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, new
 * Formula:   Signed number of partitions of n into exactly two distinct part
 *            sizes, each occurring an odd number of times. For each pair of
 *            distinct part sizes a<b, and odd positive multiplicities m,k
 *            such that a*m + b*k = n, the partition contributes (+1) if
 *            ((m-1)/2 + (k-1)/2) is even, otherwise (-1)
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Sum of signed contributions.  Use signed 64‑bit accumulator to avoid
       overflow for intermediate cancellations.  The final value fits into
       seq_t for all n up to the bounds of the problem. */
    long long sum = 0;

    for (idx_t a_part = 1; a_part < n; ++a_part)
    {
        /* m is the odd multiplicity of the small part */
        for (idx_t m = 1; a_part * m < n; m += 2)
        {
            idx_t remainder = n - a_part * m;           /* b * k = remainder */
            if (remainder == 0) continue;              /* k must be odd positive */

            /* enumerate divisors of remainder */
            for (idx_t d = 1; d * d <= remainder; ++d)
            {
                if (remainder % d != 0) continue;

                /* first divisor candidate */
                idx_t b_part = d;
                if (b_part > a_part)
                {
                    idx_t k = remainder / b_part;
                    if (k & 1ULL)   /* k must be odd */
                    {
                        int parity = ((m - 1) / 2 + (k - 1) / 2) & 1;
                        sum += (parity ? -1 : 1);
                    }
                }

                /* second divisor candidate (complement) */
                idx_t b_part2 = remainder / d;
                if (b_part2 != b_part && b_part2 > a_part)
                {
                    idx_t k = remainder / b_part2;
                    if (k & 1ULL)
                    {
                        int parity = ((m - 1) / 2 + (k - 1) / 2) & 1;
                        sum += (parity ? -1 : 1);
                    }
                }
            }
        }
    }

    return (seq_t)sum;
}
