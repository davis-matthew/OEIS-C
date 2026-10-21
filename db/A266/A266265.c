/*
 * A266265  Product of products of divisors of divisors of n.
 * https://oeis.org/A266265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} d^τ(n/d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static void rec_divisors(size_t idx, uint64_t cur, uint64_t tau_nd,
                         uint64_t *pr, uint64_t *ex, size_t cnt, seq_t *res)
{
    if (idx == cnt) {
        seq_t pow_d = big_pow((seq_t)cur, (seq_t)tau_nd);
        *res = seq_mul(*res, pow_d);
        return;
    }
    uint64_t factor = ex[idx] + 1;          /* original divisor factor for this prime */
    uint64_t cur_mul = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        uint64_t new_tau_nd = tau_nd / factor * (ex[idx] - e + 1);
        rec_divisors(idx + 1, cur * cur_mul, new_tau_nd, pr, ex, cnt, res);
        cur_mul *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset is 1; n≥1 in practice */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Initial τ(n) = ∏ (exps[i] + 1) */
    uint64_t tau_nd = 1;
    for (size_t i = 0; i < cnt; ++i) tau_nd *= (exps[i] + 1);

    seq_t result = (seq_t)1;
    rec_divisors(0, (uint64_t)1, tau_nd, primes, exps, cnt, &result);
    return result;
}
