/*
 * A345265  a(n) = Sum_{d|n} n^rad(d).
 * https://oeis.org/A345265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^rad(d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* factorize n into primes and their exponents */
    idx_t primes[64];
    idx_t exps[64];
    size_t k = 0;

    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            primes[k] = p;
            exps[k]   = exp;
            ++k;
        }
    }
    if (tmp > (idx_t)1) {
        primes[k] = tmp;
        exps[k]   = 1;
        ++k;
    }

    /* enumerate all masks of distinct prime factors */
    seq_t sum = 0;
    size_t total_masks = (size_t)1 << k;   /* k is small for n ≤ 10000 */

    for (size_t mask = 0; mask < total_masks; ++mask) {
        seq_t rad   = 1;
        seq_t count = 1;
        for (size_t i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i)) {
                rad   *= (seq_t)primes[i];
                count *= (seq_t)exps[i];
            }
        }
        seq_t term = big_pow((seq_t)n, rad);
        sum += count * term;
    }
    return sum;
}
