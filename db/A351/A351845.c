/*
 * A351845  a(n) = Sum_{d|n} Sum_{p|n, p prime} n^lcm(d,p).
 * https://oeis.org/A351845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Sum_{p|n, p prime} n^lcm(d,p)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* compute a(n) as defined in A351845 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* offset starts at 1 */
    }

    /* factorise n (list distinct prime divisors) */
    uint64_t primes[10];
    uint64_t exps[10];
    size_t prime_cnt = 0;
    uint64_t m = n;

    if ((m & 1ULL) == 0) {            /* handle factor 2 */
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        primes[prime_cnt] = 2;
        exps[prime_cnt] = e;
        ++prime_cnt;
    }
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            primes[prime_cnt] = d;
            exps[prime_cnt] = e;
            ++prime_cnt;
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        primes[prime_cnt] = m;
        exps[prime_cnt] = 1;
        ++prime_cnt;
    }

    /* generate all divisors of n */
    uint64_t divisors[512];
    size_t divisor_cnt = 1;
    divisors[0] = 1;                   /* start with divisor 1 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        size_t old_cnt = divisor_cnt;
        uint64_t p_pow = 1;
        for (unsigned int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < old_cnt; ++j) {
                divisors[divisor_cnt++] = divisors[j] * p_pow;
            }
        }
    }

    /* compute the sum */
    seq_t result = 0;
    seq_t seq_n  = (seq_t)n;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        for (size_t di = 0; di < divisor_cnt; ++di) {
            uint64_t d = divisors[di];
            seq_t exp = (seq_t)d;
            if ((d % p) != 0) {
                exp = exp * (seq_t)p;
            }
            result += big_pow(seq_n, exp);
        }
    }

    return result;
}
