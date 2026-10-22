/*
 * A369866  a(n) = Sum_{p|n, p prime} n^prime(n/p).
 * https://oeis.org/A369866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} n^prime(n/p), a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Factor the integer n into distinct prime divisors. */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t   prime_factor_cnt;

    factor_u64((uint64_t)n, primes_buf, exps_buf, &prime_factor_cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < prime_factor_cnt; ++i) {
        uint64_t p = primes_buf[i];
        idx_t    idx = n / p;                /* 1‑based index for prime(n/p) */
        if (idx == 0)                       /* should never happen for n>1 */
            continue;
        seq_t expo = get_prime(idx);          /* (n/p)-th prime */
        sum += big_pow((seq_t)n, expo);       /* n^prime(n/p) */
    }
    return sum;
}
