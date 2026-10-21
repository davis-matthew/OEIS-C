/*
 * A246600  Number of divisors d of n with property that the binary
 *          representation of d can be obtained from the binary representation
 *          of n by changing any number of 1's to 0's.
 * https://oeis.org/A246600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ d : d | n and (n & d) == d }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Satisfies OEIS A246600: number of divisors of n whose binary
   representation is obtainable from n by turning 1's into 0's.   */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursively enumerate all divisors of n and count those
   satisfying (n & d) == d.  The divisor 'cur' is built step by
   step from the exponentiation of prime factors.                */
static void
dfs(uint64_t n,
    uint64_t *primes, uint64_t *exps, size_t cnt,
    size_t pos, uint64_t cur, seq_t *out)
{
    if (pos == cnt) {
        if ((n & cur) == cur)
            (*out) += 1;
        return;
    }
    uint64_t val = cur;
    for (size_t e = 0; e <= exps[pos]; ++e) {
        dfs(n, primes, exps, cnt, pos + 1, val, out);
        if (e == exps[pos])
            break;
        val *= primes[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of range, but safe fallback */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorize n into primes and exponents */
    factor_u64(nn, primes, exps, &cnt);

    seq_t ans = 0;
    dfs(nn, primes, exps, cnt, 0, 1, &ans);
    return ans;
}
