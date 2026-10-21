/*
 * A242125  a(n) = n-th prime modulo 27.
 * https://oeis.org/A242125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime modulo 27
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function get_prime(idx_t k) which returns the k‑th prime (1‑based). */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset starts at 1 */
    return (seq_t)(get_prime(n) % 27U);
}
