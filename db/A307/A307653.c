/*
 * A307653  a(n) = Sum_{d|n} mu(d) * d^d.
 * https://oeis.org/A307653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(d) * d^d
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive helper to enumerate all divisors of n and accumulate the sum
   of mu(d) * d^d. Parameters:
   - idx: current prime factor index in the factorization arrays
   - cur: current divisor being built (uint64_t, fits in 64‑bit)
   - square: true if any prime exponent in cur is >= 2 (mu=0)
   - prime_cnt: number of prime factors in the factorization
   - cnt: number of distinct primes used with exponent 1 so far
   - pr / ex: arrays of prime bases and exponents from factor_u64
*/
static seq_t rec_sum_divisors(size_t idx, uint64_t cur, bool square,
                              int cnt, size_t prime_cnt,
                              uint64_t *pr, uint64_t *ex)
{
    if (idx == prime_cnt) {               /* leaf: cur is a divisor of n */
        if (square)                       /* mu(cur) = 0 */
            return 0;
        int mu = (cnt % 2 == 0) ? 1 : -1;  /* mu(cur) = (-1)^cnt */
        seq_t val = big_pow((seq_t)cur, (seq_t)cur); /* d^d */
        if (mu < 0)
            val = -val;
        return val;
    }

    seq_t sum = 0;
    uint64_t p  = pr[idx];
    uint64_t pow = 1;                       /* p^e for current e */

    for (size_t e = 0; e <= ex[idx]; ++e) {  /* try each exponent of p */
        uint64_t new_cur = cur * pow;
        bool new_square = square || (e >= 2);
        int new_cnt = cnt + (e == 1 ? 1 : 0);
        sum += rec_sum_divisors(idx + 1, new_cur, new_square,
                                new_cnt, prime_cnt, pr, ex);
        pow *= p;                            /* succeed to next exponent */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Factor n into primes.  The arrays are small because n <= 10000. */
    size_t   factor_cnt = 0;
    uint64_t primes[32], exps[32];
    factor_u64(n, primes, exps, &factor_cnt);

    /* Compute the sum over all divisors using recursion. */
    seq_t result = rec_sum_divisors(0, 1, false, 0,
                                    factor_cnt, primes, exps);
    return result;
}
