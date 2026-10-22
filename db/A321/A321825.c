/*
 * A321825  a(n) = Sum_{d|n, d==1 (mod 4)} d^9 - Sum_{d|n, d==3 (mod 4)} d^9.
 * https://oeis.org/A321825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d odd} d^9 * ((d%4==1)?1:-1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 128,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative definition:  a(2^e)=1.                       */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_seq.h"     /* seq_t pow_seq(seq_t base, int exp) */

static void dfs_divisors(const uint64_t *primes, const uint64_t *exps,
                         size_t idx, size_t nprimes,
                         seq_t current, seq_t *sum)
{
    if (idx == nprimes) {
        /* process divisor 'current' */
        if ((current & 1) != 0) {          /* only odd divisors contribute */
            seq_t mod4 = current % 4;
            seq_t d9   = pow_seq(current, 9); /* current^9 */
            if (mod4 == (seq_t)1)
                *sum += d9;
            else
                *sum -= d9;
        }
        return;
    }

    uint64_t p = primes[idx];
    uint64_t e = exps[idx];

    seq_t acc = current;
    /* exponent 0 */
    dfs_divisors(primes, exps, idx + 1, nprimes, acc, sum);
    /* exponents 1..e */
    for (uint64_t k = 1; k <= e; ++k) {
        acc *= (seq_t)p;
        dfs_divisors(primes, exps, idx + 1, nprimes, acc, sum);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined lower than offset */

    /* Factorization of n (up to 2^64).  At most 64 distinct primes. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs_divisors(primes, exps, 0, cnt, (seq_t)1, &sum);
    return sum;
}
