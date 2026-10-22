/*
 * A356543  a(n) = Sum_{d|n} (d!)^(n/d-1).
 * https://oeis.org/A356543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d!)^(n/d-1) (offset 1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 25, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

/* Recursive enumeration of all divisors of n.
   sum accumulates the contributions (d!)^(n/d-1). */
static void rec_divs(size_t idx, const uint64_t *primes,
                     const uint64_t *exps, size_t cnt,
                     idx_t cur, idx_t n, seq_t *sum)
{
    if (idx == cnt) {
        idx_t exp = n / cur - 1;
        seq_t term;
        if (exp == 0) {
            term = (seq_t)1;
        } else {
            seq_t fact = factorial(cur);
            term = big_pow(fact, (seq_t)exp);
        }
        *sum += term;
        return;
    }

    uint64_t p  = primes[idx];
    idx_t   mult = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        rec_divs(idx + 1, primes, exps, cnt,
                 cur * mult, n, sum);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    assert(n >= 1);

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    rec_divs(0, primes, exps, cnt, (idx_t)1, n, &sum);
    return sum;
}
