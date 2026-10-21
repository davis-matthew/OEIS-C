/*
 * A244770  Prime numbers ending in the prime number 47.
 * https://oeis.org/A244770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th prime of the form 100*k+47; found by trial
 *            search
 * Range:     u8 n <= 1, u16 n <= 129, u32 n <= 130, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    seq_t candidate;

    while (1) {
        candidate = (seq_t)(100 * k + 47);
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++k;
    }
}
