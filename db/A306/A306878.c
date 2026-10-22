/*
 * A306878  Number of 0 < k < n such that n-k and n+k are both nonprimes.
 * https://oeis.org/A306878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of 0 < k < n such that n-k and n+k are both nonprimes
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t a_val = n - k;   /* 1 <= a_val < n */
        idx_t b_val = n + k;   /* n+1 <= b_val <= 2n-1 */
        if (!is_prime(a_val) && !is_prime(b_val)) {
            ++count;
        }
    }
    return count;
}
