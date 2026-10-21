/*
 * A288420  a(n) = Sum_{d|n} d^4*A000593(n/d).
 * https://oeis.org/A288420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = Σ_{d|n} d^4 * A000593(n/d) where A000593(m) is the sum of
 *            odd divisors of m
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

static seq_t sigma_odd_of_div(uint64_t m, const uint64_t *primes, size_t cnt)
{
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;               /* skip factor 2 */
        uint64_t exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        if (exp > 0) {
            seq_t term = (pow_uint(p, exp + 1) - 1) / (p - 1);
            res *= term;
        }
    }
    return res;
}

static void rec_divs(const uint64_t *primes,
                     const uint64_t *exps,
                     size_t cnt,
                     size_t idx,
                     uint64_t curdiv,
                     uint64_t n,
                     seq_t *res)
{
    if (idx == cnt) {
        seq_t d4 = pow_uint(curdiv, 4);
        uint64_t m = n / curdiv;
        seq_t sigma = sigma_odd_of_div(m, primes, cnt);
        *res += d4 * sigma;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        rec_divs(primes, exps, cnt, idx + 1, curdiv * mult, n, res);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, but keeps the function safe */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 0;
    rec_divs(primes, exps, cnt, 0, 1, nn, &result);
    return result;
}
