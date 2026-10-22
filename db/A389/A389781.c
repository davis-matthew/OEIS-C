/*
 * A389781  Number of divisors d of n such that d^(n/d) == d (mod n).
 * https://oeis.org/A389781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d^(n/d) ≡ d (mod n)
 * Range:     u8 n <= 2419, u16 n <= 9115, u32 n <= 9115, u64 n <= 9115,
 *            u128 n <= 9115, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* recursive divisor enumeration with direct testing */
static void test_divisors(size_t idx, uint64_t cur,
                          uint64_t *pr, uint64_t *ex,
                          size_t cnt, seq_t *result,
                          uint64_t n)
{
    if (idx == cnt) {
        seq_t d = (seq_t)cur;
        seq_t mod = (seq_t)n;
        uint64_t exp = n / cur;           /* n/d  */
        seq_t r = mod_pow(d, exp, mod);

        /* d % n == d if d < n, otherwise 0 if d == n */
        seq_t dmod = d % mod;
        if (r == dmod) {
            (*result)++;
        }
        return;
    }

    /* include 0 exponent of prime */
    test_divisors(idx + 1, cur, pr, ex, cnt, result, n);

    /* use prime at this level */
    uint64_t p = pr[idx];
    uint64_t next = cur;
    for (uint64_t e = 1; e <= ex[idx]; ++e) {
        next *= p;
        test_divisors(idx + 1, next, pr, ex, cnt, result, n);
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    /* n >= 1 */
    uint64_t nn = (uint64_t)n;

    /* factor n */
    uint64_t primes[16], exps[16];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t res = 0;
    test_divisors(0, 1, primes, exps, cnt, &res, nn);
    return res;
}
