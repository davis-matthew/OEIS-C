/*
 * A340284  Number of partitions of n into 3 parts such that the 'middle' part
 *          is relatively prime to each of the other two parts.
 * https://oeis.org/A340284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} Sum_{i=k..floor((n-k)/2)}
 *            floor(1/gcd(i,k)) * * floor(1/gcd(i,n-i-k)) (OEIS A340284)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t max_i = (n - k) / 2;  /* floor((n-k)/2) */
        for (idx_t i = k; i <= max_i; ++i) {
            if (gcd((seq_t)i, (seq_t)k) == 1 &&
                gcd((seq_t)i, (seq_t)(n - k - i)) == 1) {
                ++res;
            }
        }
    }
    return res;
}
