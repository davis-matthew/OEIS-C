/*
 * A270443  Least m such that d(n^m) > n, where d(n) is the number of divisors
 *          of n.
 * https://oeis.org/A270443
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Smallest integer m ≥ 1 such that τ(n^m) > n, where τ(x)
 *            is the number of divisors of x
 * Range:     u8 n <= 244, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;      /* out of range, not needed for the OEIS file */

    /* factorization of n: n = ∏ p_i ^ e_i */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* if n is prime, answer is n */
    if (cnt == 1 && exps[0] == 1)
        return (seq_t)n;

    seq_t target = (seq_t)n;
    seq_t m = 1;
    for (;;) {
        seq_t prod = 1;
        for (size_t i = 0; i < cnt; ++i) {
            prod *= ((seq_t)exps[i] * m + 1);
        }
        if (prod > target)
            return m;
        ++m;
    }
}
