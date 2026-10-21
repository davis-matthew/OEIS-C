/*
 * A245656  Characteristic function of arithmetic numbers, cf. A003601.
 * https://oeis.org/A245656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) % tau(n) == 0, else 0 where sigma(n) = sum
 *            of divisors of n, tau(n) = number of divisors of n
 * Range:     u8 n <= 6955, u16 n <= 9790, u32 n <= 9790, u64 n <= 9790,
 *            u128 n <= 9790, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* The OEIS offset starts at n = 1. For n < 1 we simply return 0. */
    if (n < 1) return 0;

    seq_t sigma = sigma_of(n);                 /* sum of divisors */
    uint64_t tau_u64 = divisor_count_u64(n);   /* divisor count (uint64_t) */
    seq_t tau = (seq_t) tau_u64;               /* cast to seq_t for modulo */

    return (sigma % tau == 0) ? (seq_t)1 : (seq_t)0;
}
