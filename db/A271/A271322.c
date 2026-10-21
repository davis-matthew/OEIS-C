/*
 * A271322  Largest prime factor of the n-th n-gonal number (A060354).
 * https://oeis.org/A271322
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of n*(n^2 - 3n + 4)/2, offset 2
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n) {
    /* The n‑th n‑gonal number is
       P(n) = n * (n^2 - 3n + 4) / 2                     */
    uint64_t nn = (uint64_t)n;
    uint64_t P = nn * (nn * nn - 3 * nn + 4) / 2;

    /* Largest prime divisor of P, using the provided helper */
    uint64_t lpf = largest_prime_divisor(P);

    return (seq_t)lpf;
}
