/*
 * A362613  Number of co-modes in the prime factorization of n.
 * https://oeis.org/A362613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors whose exponent equals the minimum
 *            exponent in the prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t pr[32];
    uint64_t ex[32];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);
    if (cnt == 0)
        return (seq_t)0;

    uint64_t min_exp = ex[0];
    for (size_t i = 1; i < cnt; ++i)
        if (ex[i] < min_exp)
            min_exp = ex[i];

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] == min_exp)
            ++result;

    return result;
}
