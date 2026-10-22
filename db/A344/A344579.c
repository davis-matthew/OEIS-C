/*
 * A344579  a(n) = Sum_{d|n} d^sopf(d).
 * https://oeis.org/A344579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^sopf(d), where sopf(d) is the sum of distinct
 *            prime factors of d. For n=1 the only divisor is 1, whose sopf is
 *            defined to be 0, so a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* Recursive enumeration of all divisors of a number from its prime
   factorisation.  primes[0..cnt-1] are the distinct prime factors
   and exps[0..cnt-1] their exponents.  idx is the current index in
   the factor list.  curr is the product of the chosen prime powers so
   far (the current divisor), sum is the sum of distinct prime factors
   of that divisor, and *acc accumulates the sum d^sopf(d). */
static void dfs_divisors(const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, size_t idx, seq_t curr, seq_t sum,
                         seq_t *acc)
{
    if (idx == cnt) {
        seq_t term = big_pow(curr, sum);
        *acc += term;
        return;
    }

    uint64_t p = primes[idx];
    seq_t pow = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        seq_t new_div = curr * pow;
        seq_t new_sum = sum + (e > 0 ? (seq_t)p : (seq_t)0);
        dfs_divisors(primes, exps, cnt, idx + 1, new_div, new_sum, acc);
        pow = pow * (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n = 0, but safe to return 0 */
        return 0;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {        /* tmp itself is a prime factor */
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t total = 0;
    dfs_divisors(primes, exps, cnt, 0, (seq_t)1, (seq_t)0, &total);
    return total;
}
