/*
 * A369740  a(n) = Sum_{p|n, p prime} p^sigma(n/p).
 * https://oeis.org/A369740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p^sigma(n/p)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 35, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 */
    if (n <= 1u)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n into distinct primes */
    factor_u64(N, primes, exps, &cnt);

    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        idx_t m = n / (idx_t)p;                /* n/p */
        seq_t sigma = sigma_of(m);              /* sigma(n/p) */
        seq_t term  = big_pow((seq_t)p, sigma); /* p^sigma(n/p) */
        sum += term;
    }
    return sum;
}
