/*
 * A304081  Number of ways to write n as p + 2^k + (1+(n mod 2))*5^m, where p
 *          is an odd prime, and k and m are nonnegative integers with 2^k +
 *          (1+(n mod 2))*5^m squarefree.
 * https://oeis.org/A304081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of ways to express n as p + 2^k + (1 + n mod 2)*5^m *
 *            where p is an odd prime, k,m≥0, and 2^k + (1 + n mod 2)*5^m is
 *            squarefree
 * Range:     u8 n <= 667, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    /* Convert index to uint64_t for arithmetic */
    uint64_t N = (uint64_t)n;
    /* factor = 1 + (n mod 2): 1 for even n, 2 for odd n */
    uint64_t factor = 1 + (N & 1ULL);

    seq_t count = 0;

    /* iterate over powers of two: 2^k */
    uint64_t pow2 = 1ULL;
    for (uint64_t k = 0; pow2 < N; ++k, pow2 <<= 1) {
        /* For each 2^k, iterate over powers of five: 5^m */
        uint64_t pow5 = 1ULL;
        for (uint64_t m = 0;; ++m, pow5 *= 5ULL) {
            /* Compute sum = 2^k + factor * 5^m */
            uint64_t sum = pow2 + factor * pow5;
            if (sum >= N) break;              /* p would be <= 0 */
            /* Check squarefree of the sum */
            if (!is_squarefree((seq_t)sum)) continue;

            uint64_t p = N - sum;
            if (p > 2ULL && (p & 1ULL) && is_prime_small((seq_t)p))
                ++count;
        }
    }
    return count;
}
