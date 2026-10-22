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
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (k & 1ULL) {                   // odd k → σ(k)
            sum += sigma_of(k);
        } else {                           // even k → τ(k)
            /* divisor_count_u64 returns an unsigned 64‑bit integer.
               It can be safely converted to seq_t. */
            sum -= (seq_t)divisor_count_u64(k);
        }
    }
    return sum;
}
