/*
 * A389162  a(n) = A034448(n) - A048250(n), where A034448 is the sum of
 *          unitary divisors and A048250 is the sum of squarefree divisors.
 * https://oeis.org/A389162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of unitary divisors of n minus sum of squarefree
 *            divisors of n
 * Range:     u8 n <= 239, u16 n <= 9443, u32 n <= 9443, u64 n <= 9443,
 *            u128 n <= 9443, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Sum of unitary divisors of n */
static seq_t unitary_sum(uint64_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p  = (seq_t)pr[i];
        seq_t pe = (seq_t)1;
        for (uint64_t j = 0; j < ex[i]; ++j)
            pe *= p;
        res *= (seq_t)1 + pe;
    }
    return res;
}

/* Sum of squarefree divisors of n */
static seq_t squarefree_sum(uint64_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)pr[i];
        res *= (seq_t)1 + p;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t u = (uint64_t)n;
    seq_t uni = unitary_sum(u);
    seq_t sf  = squarefree_sum(u);
    return uni - sf;
}
