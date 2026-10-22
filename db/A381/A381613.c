/*
 * A381613  If n = Product (p_j^k_j) then a(n) = Product (min(p_j, k_j)), with
 *          a(1) = 1.
 * https://oeis.org/A381613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏ min(pᵢ, kᵢ) over the prime factorisation of n (=∏
 *            pᵢ^kᵢ), with a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }

    idx_t tmp = n;
    seq_t result = 1;

    /* factor 2 separately */
    idx_t count = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        count++;
    }
    if (count) {
        result *= (seq_t)((2ULL < count) ? 2ULL : count);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            count = 0;
            while (tmp % p == 0) {
                tmp /= p;
                count++;
            }
            result *= (seq_t)((p < count) ? p : count);
        }
    }

    /* if remainder is a prime > 1 */
    if (tmp > 1) {
        result *= (seq_t)1;  /* min(tmp,1) = 1 */
    }

    return result;
}
