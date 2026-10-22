/*
 * A385138  The sum of divisors d of n such that n/d is a 5-rough number
 *          (A007310).
 * https://oeis.org/A385138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors d of n such that n/d is 5‑rough
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* static cache of primes up to the largest limit requested */
static uint64_t *primes = NULL;    /* array of primes */
static size_t   prime_cnt = 0;     /* number of primes stored */
static idx_t    prime_limit = 0;   /* maximum prime value stored */

/* Ensure that all primes ≤ limit are available in the array.
   (Nothing is done for limit < 2.) */
static void
ensure_primes(idx_t limit)
{
    if (limit < 2) return;
    if (limit <= prime_limit) return;
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Helper: convert unsigned long long to seq_t using pow_uint where
   necessary; here we only need simple arithmetic, so no helper needed. */

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; guard anyway */

    uint64_t nn = (uint64_t)n;     /* working copy of n */
    seq_t    res = 1;              /* multiplicative accumulator */

    /* compute floor(sqrt(nn)) by simple integer loop (nn ≤ 10⁴ in our tests) */
    idx_t sqrt_m = 0;
    for (idx_t t = 1; (t * (uint64_t)t) <= nn; ++t) sqrt_m = t;

    /* make sure primes up to sqrt_m are available */
    ensure_primes(sqrt_m);

    /* factorise nn using the prime list */
    for (size_t i = 0; i < prime_cnt && primes[i] <= sqrt_m; ++i) {
        uint64_t p = primes[i];
        if (nn % p == 0) {
            int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }

            seq_t term;
            if (p <= 3u) {                    /* p = 2 or 3  →  a(p^e) = p^e */
                /* use pow_uint when available; otherwise loop */
                term = 1;
                for (int j = 0; j < exp; ++j) term *= (seq_t)p;
            } else {                          /* p ≥ 5 →  sum_{k=0..e} p^k */
                seq_t sum  = 1;                 /* p^0 */
                seq_t power = 1;
                for (int j = 0; j < exp; ++j) {
                    power *= (seq_t)p;
                    sum   += power;
                }
                term = sum;
            }
            res *= term;
        }
    }

    /* any remaining factor > sqrt(nn) is a prime with exponent 1 */
    if (nn > 1) {
        uint64_t p = nn;
        seq_t term;
        if (p <= 3u) {
            term = (seq_t)p;          /* p^1 */
        } else {
            term = (seq_t)p + 1;      /* p + 1 = sum_{k=0}^1 p^k */
        }
        res *= term;
    }

    return res;
}
