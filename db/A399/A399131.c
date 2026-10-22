/*
 * A399131  Number of moves to solve the Tower of Hanoi whose disks are the
 *          prime factors of n.
 * https://oeis.org/A399131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..k} e_i * 2^(k-i) where the primes p_1 < p_2 <
 *            ... < p_k are the distinct prime divisors of n and e_i is the
 *            exponent of p_i in n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    /* maximum number of distinct prime factors of a 64‑bit integer
       is less than 64, so a fixed-size array is safe */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor the input, obtaining primes[i] and exps[i] */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* weight = 2^(cnt-1-i) */
        seq_t weight = pow_seq((seq_t)2, (idx_t)(cnt - 1 - i));
        /* term = exps[i] * weight */
        seq_t term = mul_small(weight, (unsigned int)exps[i]);
        result += term;
    }
    return result;
}
