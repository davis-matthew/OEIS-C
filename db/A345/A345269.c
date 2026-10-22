/*
 * A345269  a(n) = Sum_{d|n} (n/d)^(phi(n/d) - 1).
 * https://oeis.org/A345269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(phi(n/d)-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 18, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"

static void rec_divisors(size_t idx, uint64_t current,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, idx_t n, seq_t *sum)
{
    if (idx == cnt) {
        /* current is a divisor d of n, compute m = n/d */
        seq_t m = (seq_t)(n / current);
        seq_t phi_m = calc_phi(m);
        seq_t exp = phi_m - (seq_t)1;           /* exponent = phi(m)-1 */
        seq_t term = big_pow(m, exp);           /* m^exp */
        *sum += term;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        rec_divisors(idx + 1, current * val,
                     primes, exps, cnt, n, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    rec_divisors(0, (uint64_t)1, primes, exps, cnt, n, &sum);
    return sum;
}
