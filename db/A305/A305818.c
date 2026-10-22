/*
 * A305818  Number of proper divisors d of n such that 2d+1 is a prime.
 * https://oeis.org/A305818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors d of n such that 2d+1 is prime
 * Range:     u8 n <= 681, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* seq_t a(idx_t n)
 * Computes the A305818 sequence.
 */
seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n <= 1) {
        /* n = 1 has no proper divisors */
        return count;
    }

    /* For n up to the typical limits (≈10⁴) a simple linear search is
     * fast enough. */
    for (idx_t d = 1; d < n; ++d) {
        if (n % d == 0) {
            /* 2*d+1 fits in idx_t for the values we consider. */
            seq_t v = (seq_t)2 * (seq_t)d + 1;
            if (is_prime((idx_t)v)) {
                ++count;
            }
        }
    }

    return count;
}
