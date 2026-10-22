/*
 * A340280  Number of partitions of n into 3 parts such that the 'middle' part
 *          is relatively prime to at least one other part.
 * https://oeis.org/A340280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 parts (a ≤ b ≤ c,
 *            a+b+c=n) such that the middle part b is relatively prime to at
 *            least one of the other parts (i.e., gcd(b,a)=1 or gcd(b,c)=1).
 *            Implementation enumerates all admissible triples
 * Range:     u8 n <= 58, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is ≥ 1. */
    seq_t count = 0;
    idx_t max_a = n / 3;          /* smallest part ≤ n/3 */
    for (idx_t a = 1; a <= max_a; ++a)
    {
        idx_t max_b = (n - a) / 2;    /* a ≤ b ≤ (n–a)/2 ensures c ≥ b */
        for (idx_t b = a; b <= max_b; ++b)
        {
            idx_t c = n - a - b;      /* c ≥ b by construction */
            if (gcd((seq_t)b, (seq_t)a) == 1) {
                ++count;
                continue;
            }
            if (gcd((seq_t)b, (seq_t)c) == 1) {
                ++count;
            }
        }
    }
    return count;
}
