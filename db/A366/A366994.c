/*
 * A366994  The largest divisor of n that is not a term of A322448.
 * https://oeis.org/A366994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ p_i^{g(e_i)} for n = ∏ p_i^{e_i} where g(e) = e if e =
 *            1 or e is prime largest prime ≤ e otherwise i.e. for each prime
 *            factor exponent we keep it if it is 1 or a prime, otherwise we
 *            reduce it to the greatest prime not exceeding it, and multiply
 *            the resulting primes
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* small primes up to 61 are enough for all exponents that occur in an
   unsigned 64‑bit integer (max exponent is 63 for 2^63). */
static const int SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61
};

/* true if x is a prime < 200 (enough for our small exponents). */
static bool isPrimeSmall(int x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]); ++i) {
        int p = SMALL_PRIMES[i];
        if (p == x) return true;
        if (p > x) break;
    }
    return false;
}

/* largest prime <= e (e ≥ 2). */
static int largestPrimeLE(int e)
{
    for (int x = e; x >= 2; --x) {
        if (isPrimeSmall(x)) return x;
    }
    return 1;  /* never reached for e ≥ 2 */
}

seq_t a(idx_t n)
{
    /* for n = 1 we directly return 1; other arguments are >= 1. */
    if (n == 1) return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];
        int f = e;
        if (e > 1 && !isPrimeSmall(e)) {
            f = largestPrimeLE(e);
        }
        seq_t pow = seq_pow((seq_t)p, (idx_t)f);
        result *= pow;
    }
    return result;
}
