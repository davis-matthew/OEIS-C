/*
 * A345468  a(n) is the least number k such that k^2+4*n is prime.
 * https://oeis.org/A345468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: least k such that k^2 + 4*n is prime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)  // offset starts at 1; return 0 for invalid indices
        return (seq_t)0;

    seq_t four_n = (seq_t)4 * (seq_t)n;
    for (seq_t k = (seq_t)1;; ++k) {
        seq_t val = k * k + four_n;
        if (is_prime_big(val))
            return k;
    }
}
