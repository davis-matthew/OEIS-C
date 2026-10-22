/*
 * A346010  a(n) is the denominator of the average number of distinct prime
 *          factors of the divisors of n.
 * https://oeis.org/A346010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = τ(n) / gcd( τ(n), Σ_i e_i * Π_{j≠i}(e_j+1) ), where τ(n)
 *            is the divisor count, n=Π p_i^{e_i}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥1 */
    uint64_t nn = (uint64_t)n;

    /* Factor n: store primes and exponents */
    size_t cnt = 0;
    uint64_t pr[16], ex[16];
    factor_u64(nn, pr, ex, &cnt);

    /* Compute τ(n) and Σ ω(d) over all divisors */
    uint64_t tau64 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau64 *= (uint64_t)ex[i] + 1ULL;
    }

    uint64_t sum64 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prod_other = tau64 / ((uint64_t)ex[i] + 1ULL);
        uint64_t rad = (uint64_t)ex[i] * prod_other;
        sum64 += rad;
    }

    /* Convert to seq_t and reduce */
    seq_t tau_s  = (seq_t)tau64;
    seq_t sum_s  = (seq_t)sum64;
    seq_t g      = gcd(tau_s, sum_s);

    return tau_s / g;
}
