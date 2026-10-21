/*
 * A295306  a(n) = +1 if tau(sigma(n)) > tau(n), -1 if n < tau(sigma(n)) <
 *          tau(n), and 0 if tau(sigma(n)) = tau(n), where tau = A000005 and
 *          sigma = A000203.
 * https://oeis.org/A295306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign( tau(sigma(n)) - tau(n) )
 * Range:     i8 n <= 71, i16 n <= 9211, i32 n <= 9211, i64 n <= 9211,
 *            i128 n <= 9211, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau = A000005 (divisor count), sigma = A000203 (sum of divisors) */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"  /* divisor_count_u64(uint64_t) -> uint64_t */
#include "../utils/sigma_of.h"          /* sigma_of(idx_t) -> seq_t */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 for safety */

    /* τ(n) */
    uint64_t tau_n = divisor_count_u64((uint64_t)n);

    /* σ(n) */
    seq_t sigma_n = sigma_of(n);

    /* τ(σ(n)) – σ(n) fits in 64‑bit for n ≤ 10000, cast safely */
    uint64_t sigma_val = (uint64_t)sigma_n;
    uint64_t tau_sigma = divisor_count_u64(sigma_val);

    seq_t diff = (seq_t)tau_sigma - (seq_t)tau_n;

    if (diff > 0) return (seq_t)1;
    if (diff < 0) return (seq_t)-1;
    return (seq_t)0;
}
