/*
 * A334874  a(n) = sigma(1) - tau(2) + sigma(3) - tau(4) + sigma(5) - tau(6) +
 *          ... - (up to n).
 * https://oeis.org/A334874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} (-1)^{k+1} * (k odd ? σ(k) : τ(k)) where σ(k)
 *            is the sum of divisors and τ(k) the number of divisors
 * Range:     i8 n <= 22, i16 n <= 332, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9939 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (k & 1ULL) {                   // odd k → σ(k)
            sum += sigma_of(k);
        } else {                           // even k → τ(k)
            /* divisor_count_u64 returns an unsigned 64‑bit integer.
               It can be safely converted to seq_t. */
            sum -= (seq_t)num_divisors(k);
        }
    }
    return sum;
}
