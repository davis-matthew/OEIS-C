/*
 * A340286  Number of partitions of n into 3 parts such that at least two
 *          parts share a common factor > 1.
 * https://oeis.org/A340286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into 3 positive parts
 *            such that at least two parts share a common factor > 1.
 *            Enumerate all triples (k, i, j) with k ≤ i ≤ j, k + i + j = n
 *            and count those where gcd(k,i)>1 or gcd(k,j)>1 or gcd(i,j)>1
 * Range:     u8 n <= 60, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t maxi = (n - k) / 2;
        for (idx_t i = k; i <= maxi; ++i)
        {
            idx_t j = n - k - i;
            seq_t g1 = gcd((seq_t)k, (seq_t)i);
            if (g1 > 1)
            {
                ++count;
                continue;
            }
            seq_t g2 = gcd((seq_t)k, (seq_t)j);
            if (g2 > 1)
            {
                ++count;
                continue;
            }
            seq_t g3 = gcd((seq_t)i, (seq_t)j);
            if (g3 > 1)
            {
                ++count;
            }
        }
    }
    return count;
}
