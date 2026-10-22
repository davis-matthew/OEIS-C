/*
 * A380811  Smallest composite number divisible by prime(n) which shares at
 *          least one decimal digit with prime(n).
 * https://oeis.org/A380811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A380811:  a(n) = smallest composite number divisible by
 * prime(n) that shares at least one decimal digit with prime(n).
 * The algorithm checks successive multiples of the prime until a share
 * of digits is found. */

#include <stdbool.h>
#include "../utils/get_prime.h"

/* Additionally, the following helper utilities are available but not
 * required for this implementation: digit_counts, build_primes, etc. */

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 (out of OEIS offset).  The specification
     * allows arbitrary behaviour below the offset. */
    if (n < 1)
        return 0;

    /* Obtain the n-th prime.  The helper function is provided by the
     * build.  It may allocate a cache internally, which is fine. */
    uint64_t p = get_prime(n);

    /* Pre‑compute which decimal digits appear in the prime. */
    bool p_digits[10] = { false };
    uint64_t tmp = p;
    do {
        p_digits[tmp % 10] = true;
        tmp /= 10;
    } while (tmp > 0);

    /* Check successive multiples p*k, starting with k = 2.  Since k≥2,
     * the product is guaranteed to be composite.  We stop at the first
     * multiple that shares any digit with the prime. */
    for (idx_t k = 2;; ++k) {
        seq_t m = (seq_t)p * (seq_t)k;

        /* Test whether m shares any digit with the prime. */
        seq_t mm = m;
        bool shares = false;
        do {
            uint8_t digit = (uint8_t)(mm % 10);
            if (p_digits[digit]) {
                shares = true;
                break;
            }
            mm /= 10;
        } while (mm > 0);

        if (shares)
            return m;
    }
}
