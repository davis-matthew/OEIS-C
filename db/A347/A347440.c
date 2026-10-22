/*
 * A347440  Number of factorizations of n with alternating product < 1.
 * https://oeis.org/A347440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly increasing factorizations of n into
 *            integers >1 whose alternating product (product of odd‐indexed
 *            terms divided by product of even‐indexed terms) is < 1.
 *            Implementation uses recursive enumeration of factorizations
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Recursive helper: n  current product to factor,
         minf  minimal factor allowed (to enforce weakly increasing order),
         oddNext  true if next factor will be in odd position,
         oddP, evenP  running products of odd and even positioned factors
         cnt pointer to accumulate count of valid factorizations. */
static void dfs(uint64_t n, uint64_t minf, bool oddNext,
                uint64_t oddP, uint64_t evenP, seq_t *cnt)
{
    for (uint64_t f = minf; f <= n; ++f)
    {
        if (n % f != 0) continue;
        uint64_t new_n  = n / f;
        uint64_t newOddP = oddP;
        uint64_t newEvenP = evenP;
        if (oddNext) newOddP *= f; else newEvenP *= f;

        if (new_n == 1)
        {
            if (newOddP < newEvenP) ++(*cnt);
        }
        else
        {
            dfs(new_n, f, !oddNext, newOddP, newEvenP, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence defined for n ≥ 1 */

    seq_t cnt = 0;
    dfs((uint64_t)n, 2, true, 1, 1, &cnt);
    return cnt;
}
