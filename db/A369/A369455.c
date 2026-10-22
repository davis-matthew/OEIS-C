/*
 * A369455  Dirichlet convolution of A083345 with A055615 (Dirichlet inverse
 *          of n), where A083345(n) = (n'/gcd(n,n')) and n' is the arithmetic
 *          derivative of n.
 * https://oeis.org/A369455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A055615(n/d) * A083345(d)
 * Range:     i8 n <= 63, i16 n <= 9758, i32 n <= 9758, i64 n <= 9758,
 *            i128 n <= 9758, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Arithmetic derivative of n: n' = Σ e_i * (n / p_i) */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nval = (seq_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)exps[i] * (nval / (seq_t)primes[i]);
    }
    return sum;
}

/* A083345(n) = n' / gcd(n, n') */
static seq_t a083345_single(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nval = (seq_t)n;
    seq_t der = arithmetic_derivative(n);
    seq_t g = gcd(der, nval);
    return der / g;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, but safe */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 0;
    size_t subsets = 1u << cnt;
    for (size_t mask = 0; mask < subsets; ++mask) {
        idx_t m = 1;
        int bits = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1u << i)) {
                m *= (idx_t)primes[i];
                ++bits;
            }
        }
        seq_t sign = (bits & 1) ? -1 : 1;

        idx_t d = n / m;           /* n/m is always integer */
        seq_t val = a083345_single(d);

        seq_t term = val * (seq_t)m;
        if (sign == -1) term = -term;
        result += term;
    }
    return result;
}
