/*
 * A243917  Number of non-twin divisors of n.
 * https://oeis.org/A243917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑twin divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_non_twin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_non_twin.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is at least 1 for the OEIS offset; guard anyway */
    if (n == 0)
        return (seq_t)0;

    uint64_t un = (uint64_t)n;
    seq_t cnt = 0;

    /* enumerate all divisors by iterating up to sqrt(n) */
    for (uint64_t i = 1;
         (__uint128_t)i * (__uint128_t)i <= un;
         ++i)
    {
        if (un % i == 0)
        {
            add_non_twin(i, un, &cnt);
            uint64_t j = un / i;
            if (j != i)
                add_non_twin(j, un, &cnt);
        }
    }
    return cnt;
}
