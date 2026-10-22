/*
 * A345263  a(n) = Sum_{d|n} d^rad(d).
 * https://oeis.org/A345263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^rad(d) (OEIS A345263)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, factor_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/rad.h"

/* Recursive helper: generate all divisors of n.
   `idx`   : current prime factor index
   `pf_cnt`: number of distinct prime factors
   `primes`/`exps` : prime factorization of n
   `val`  : current divisor value
   `rad`  : product of distinct primes already used in `val`
   `sum`  : accumulated result                      */
static void dfs_divisors(size_t idx, size_t pf_cnt,
                         const uint64_t *primes, const uint64_t *exps,
                         seq_t val, seq_t rad, seq_t *sum)
{
    if (idx == pf_cnt) {
        /* Add d^rad(d) for the divisor `val`                 */
        *sum += big_pow(val, rad);
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t emax= exps[idx];

    /* e = 0 case: prime p not used in this divisor        */
    dfs_divisors(idx + 1, pf_cnt, primes, exps, val, rad, sum);

    /* e >= 1 cases: prime p used at least once            */
    seq_t rad_ep = rad * (seq_t)p;  /* rad contains p once             */
    seq_t val_ep = val;
    for (uint64_t e = 1; e <= emax; ++e) {
        val_ep *= (seq_t)p;
        dfs_divisors(idx + 1, pf_cnt, primes, exps, val_ep, rad_ep, sum);
    }
}

seq_t a(idx_t n)
{
    /* Empty or invalid index: safeguard */
    if (n == 0) return 0;

    /* Factorise n: primes[] holds distinct primes,
       exps[] holds their exponents.                */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    seq_t sum = 0;
    dfs_divisors(0, pf_cnt, primes, exps, (seq_t)1, (seq_t)1, &sum);

    return sum;
}
