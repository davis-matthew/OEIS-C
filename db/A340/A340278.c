/*
 * A340278  Number of partitions of n into 3 parts whose smallest and largest
 *          parts are relatively prime.
 * https://oeis.org/A340278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/3⌋} Sum_{i=k..⌊(n-k)/2⌋} [gcd(k, n-i-k)=1]
 *            where [P] is 1 if property P holds and 0 otherwise
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n < 3)                 /* partitions of size <3 are impossible */
        return count;

    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t max_i = (n - k) / 2;      /* ensures j >= i */
        for (idx_t i = k; i <= max_i; ++i) {
            idx_t j = n - k - i;        /* third part, j >= i */
            if (gcd((seq_t)k, (seq_t)j) == (seq_t)1)
                ++count;
        }
    }

    return count;
}
