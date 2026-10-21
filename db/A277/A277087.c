/*
 * A277087  a(0) = 1, a(n) = (denominator of the Bernoulli number B_{2n})/3,
 *          for n>=1.
 * https://oeis.org/A277087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1; a(n) = (denominator of B_{2n})/3 for n>=1
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 89, u64 n <= 269,
 *            u128 n <= 1889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for 64‑bit integers */
static inline bool is_prime_uint64(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t m = n * (idx_t)2;          /* 2n */
    seq_t product = (seq_t)1;

    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t p = i + 1;
            if (is_prime_uint64(p)) product *= (seq_t)p;

            idx_t j = m / i;
            if (j != i) {
                idx_t q = j + 1;
                if (is_prime_uint64(q)) product *= (seq_t)q;
            }
        }
    }
    return product / (seq_t)3;
}
