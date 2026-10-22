/*
 * A340275  Number of partitions of n into 3 parts whose smallest two parts
 *          are relatively prime.
 * https://oeis.org/A340275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} Sum_{i=k..floor((n-k)/2)}
 *            [gcd(i,k)==1]
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;
    seq_t count = 0;
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t max_i = (n - k) / 2;
        for (idx_t i = k; i <= max_i; ++i) {
            if (gcd((seq_t)i, (seq_t)k) == (seq_t)1)
                ++count;
        }
    }
    return count;
}
