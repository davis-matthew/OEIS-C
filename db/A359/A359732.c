/*
 * A359732  a(n) = Sum_{d|n} d^(2*d-1).
 * https://oeis.org/A359732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} d^(2*d-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* recursively generate all divisors of n from its prime factorization */
static void gen_divs(uint64_t *primes, uint64_t *exps, size_t idx,
                     size_t np, uint64_t curr, uint64_t *divs, size_t *cnt)
{
    if (idx == np) {
        divs[(*cnt)++] = curr;
        return;
    }
    uint64_t p   = primes[idx];
    uint64_t emax = exps[idx];
    uint64_t v = curr;
    for (size_t e = 0; e <= emax; ++e) {
        gen_divs(primes, exps, idx + 1, np, v, divs, cnt);
        v *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* for safety; sequence defined for n≥1 */

    uint64_t nn = (uint64_t)n;

    /* factor n: primes[i]^exps[i] */
    uint64_t primes[32];
    uint64_t exps [32];
    size_t   pcount = 0;
    factor_u64(nn, primes, exps, &pcount);

    /* enumerate all divisors */
    uint64_t divs[512];
    size_t   divcnt = 0;
    gen_divs(primes, exps, 0, pcount, 1ULL, divs, &divcnt);

    /* compute the sum of d^(2*d-1) over all divisors d */
    seq_t sum = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        seq_t d   = (seq_t)divs[i];
        uint64_t e = 2 * divs[i] - 1;          /* exponent fits in 64‑bit */
        seq_t exp = (seq_t)e;
        seq_t term = big_pow(d, exp);
        sum += term;
    }
    return sum;
}
