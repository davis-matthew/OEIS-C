/*
 * A336158  The least number with the prime signature of the odd part of n:
 *          a(n) = A046523(A000265(n)).
 * https://oeis.org/A336158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A000265(n)) that is, a(n) = minimal number whose
 *            prime signature equals that of the odd part of n
 * Range:     u8 n <= 3464, u16 n <= 9388, u32 n <= 9388, u64 n <= 9388,
 *            u128 n <= 9388, big any n
 * Helpers:   build_primes_upto, factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure that there are at least `min_cnt` primes available in `primes`. */
static void ensure_primes(size_t min_cnt)
{
    if (primes_cnt >= min_cnt)
        return;

    /* Rough upper bound for the min-th prime: p_n < n*(log n + log log n) for n >= 6. */
    idx_t limit = 20;  /* default small limit */
    if (min_cnt > 10)
        limit = (idx_t)(min_cnt * 20);

    /* Grow until we have enough primes. */
    while (true) {
        size_t old_cnt = primes_cnt;
        build_primes_upto(limit, &primes, &primes_cnt);
        if (primes_cnt >= min_cnt)
            break;
        /* Increase limit and retry. */
        limit *= 2;
        /* Free old array if it was reallocated by the function. */
        if (primes_cnt > old_cnt && primes) {
            free(primes);
            primes = NULL;
        }
    }
}

/* Power: seq_t base^exp, with exp an unsigned integer. */
static seq_t pow_uint(uint64_t base, unsigned exp)
{
    seq_t r = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1)
            r *= b;
        b *= b;
        exp >>= 1;
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1. odd part of n */
    uint64_t odds = (uint64_t)n;
    while ((odds & 1) == 0)
        odds >>= 1;

    if (odds == 1)   /* a(1)=a(2)=... where odd part is 1 */
        return (seq_t)1;

    /* 2. factorize odd part and collect exponents */
    uint64_t primes_f[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(odds, primes_f, exps, &cnt);

    /* 3. sort exponents descending */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (exps[i] < exps[j]) {
                uint64_t tmp = exps[i];
                exps[i]  = exps[j];
                exps[j]  = tmp;
            }

    /* 4. ensure we have enough small primes to assign. */
    ensure_primes(cnt);

    /* 5. build the minimal number: product of the first `cnt` primes to the sorted exponents */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= pow_uint(primes[i], (unsigned)exps[i]);

    return result;
}
