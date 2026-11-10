/*
 * A266400  Indices of primes in A005097.
 * https://oeis.org/A266400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that (prime(k+1)-1)/2 is prime; computed by a
 *            search over odd primes
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 974 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0; /* prime(k+1) = q, k counts odd primes seen so far */
    for (uint64_t q = 3; ; q += 2) {
        if (is_prime_u64(q)) {
            k++;
            if (is_prime_u64((q - 1) / 2)) {
                count++;
                if (count == n) return (seq_t)k;
            }
        }
    }
}
