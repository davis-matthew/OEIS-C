/*
 * A303695  a(n) is the sum of divisors d of n such that bigomega(d) is a
 *          divisor of bigomega(n) where bigomega(x) is A001222(x), the number
 *          of prime divisors of x counted with multiplicity.
 * https://oeis.org/A303695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A303695: a(n) = sum_{d|n, bigomega(d) | bigomega(n)} d  */
/*  The function uses the supplied bigomega and factor_u64 utilities.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"

/*  Recursive helper: generate all divisors of n.
    cur      – current divisor being built
    curOmega – number of prime factors (with multiplicity) of cur
    idx      – index of the prime factor we are processing
    prvCnt   – total number of distinct primes in the factorisation
    primes   – array of prime factors of n
    exp      – array of exponents of those primes
    sum      – accumulated sum of qualifying divisors
    OmegaN   – bigomega(n)  */
static void
dfs_divisors(unsigned idx, const uint64_t *primes, const uint64_t *exp,
             unsigned prvCnt, uint64_t cur, unsigned curOmega,
             seq_t OmegaN, seq_t *sum)
{
    if (idx == prvCnt) {
        /* Include cur if its bigomega (curOmega) divides bigomega(n) */
        if (curOmega > 0 && (OmegaN % (seq_t)curOmega) == 0)
            *sum += (seq_t)cur;
        return;
    }

    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (unsigned e = 0; e <= exp[idx]; ++e) {
        dfs_divisors(idx + 1, primes, exp, prvCnt, val, curOmega + e,
                     OmegaN, sum);
        /* Prepare val for e+1 (ensure 128‑bit to avoid overflow) */
        if (e < exp[idx]) {
            __uint128_t t = ( __uint128_t) val * p;
            val = (uint64_t) t;
        }
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t OmegaN = bigomega(n);   /* bigomega(n) */

    /* Factorisation of n
       primes[]  – distinct prime factors
       exp[]     – corresponding exponents
       cnt       – number of distinct prime factors   */
    uint64_t primes[64];
    uint64_t exp[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exp, &cnt);

    dfs_divisors(0, primes, exp, (unsigned)cnt, 1ULL, 0, OmegaN, &sum);
    return sum;
}
