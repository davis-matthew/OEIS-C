/*
 * A329308  a(n) is the number of k with 1 < k < sqrt(n) such that n mod k^2
 *          is prime.
 * https://oeis.org/A329308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count k with 1 < k < sqrt(n) such that n mod k^2 is prime */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 2; k * k < n; ++k) {
        idx_t rem = n % (k * k);
        if (is_prime(rem))
            ++cnt;
    }
    return cnt;
}
