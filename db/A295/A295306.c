/*
 * A295306  a(n) = +1 if tau(sigma(n)) > tau(n), -1 if n < tau(sigma(n)) <
 *          tau(n), and 0 if tau(sigma(n)) = tau(n), where tau = A000005 and
 *          sigma = A000203.
 * https://oeis.org/A295306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign( tau(sigma(n)) - tau(n) )
 * Range:     i8 n <= 71, i16 n <= 65536, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9211 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau = A000005 (divisor count), sigma = A000203 (sum of divisors) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 for safety */

    /* τ(n) */
    uint64_t tau_n = num_divisors((uint64_t)n);

    /* σ(n) */
    seq_t sigma_n = sigma_of(n);

    /* τ(σ(n)) – σ(n) fits in 64‑bit for n ≤ 10000, cast safely */
    uint64_t sigma_val = (uint64_t)sigma_n;
    uint64_t tau_sigma = num_divisors(sigma_val);

    seq_t diff = (seq_t)tau_sigma - (seq_t)tau_n;

    if (diff > 0) return (seq_t)1;
    if (diff < 0) return (seq_t)-1;
    return (seq_t)0;
}
