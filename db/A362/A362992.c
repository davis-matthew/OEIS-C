/*
 * A362992  a(n) = (n + 1)^(n - 1) * lcm{k + 1 : 0 <= k <= n}. Main diagonal
 *          of triangle A362995.
 * https://oeis.org/A362992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)^(n - 1) * lcm{k + 1 : 0 <= k <= n}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper imports from utils (assumed available) */
#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_buf = NULL;   /* cached sieve array */
static size_t prime_cnt   = 0;       /* number of primes in buffer */
static idx_t  max_checked = 0;       /* largest n+1 for which primes were generated */

/* Computes a(n) = (n + 1)^(n - 1) * lcm(1, 2, ..., n + 1) */
seq_t a(idx_t n)
{
    /* Special case: OEIS defines a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Prepare primes up to n + 1 (inclusive) if not already available */
    idx_t target = n + 1;
    if (target > max_checked) {
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        build_primes_upto(target, &new_primes, &new_cnt);

        free(prime_buf);          /* discard old sieve array if any */
        prime_buf = new_primes;
        prime_cnt = new_cnt;
        max_checked = target;     /* updated maximum */
    }

    /* Compute L = lcm(1, 2, ..., n + 1) as product of prime powers */
    seq_t lcm = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && prime_buf[i] <= target; ++i) {
        uint64_t p = prime_buf[i];
        /* Find highest e such that p^e <= target */
        idx_t e = 0;
        unsigned long long pow = p;
        while (pow <= target) {
            pow *= p;
            ++e;
        }
        /* Multiply lcm by p^e */
        seq_t pe = big_pow((seq_t)p, (seq_t)e);
        lcm = seq_mul(lcm, pe);
    }

    /* Compute (n + 1)^(n - 1) */
    seq_t base   = (seq_t)(n + 1);
    seq_t exp    = (seq_t)(n - 1);
    seq_t pownum = big_pow(base, exp);

    /* Final result: pownum * lcm */
    seq_t result = seq_mul(pownum, lcm);
    return result;
}
