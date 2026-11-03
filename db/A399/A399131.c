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
 * Validated: standard, pass, 106 terms through n = 9199 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
        seq_t weight = ipow_i32((seq_t)2, (idx_t)(cnt - 1 - i));
        /* term = exps[i] * weight */
        seq_t term = mul_small(weight, (unsigned int)exps[i]);
        result += term;
    }
    return result;
}
