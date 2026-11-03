/*
 * A243917  Number of non-twin divisors of n.
 * https://oeis.org/A243917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑twin divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8720 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a divisor k of n is a non‑twin divisor. */
static inline void add_non_twin(uint64_t k, uint64_t n, seq_t *count)
{
    bool twin = false;
    /* k - 2 > 0 is equivalent to k > 2 */
    if (k > 2 && n % (k - 2) == 0)
        twin = true;
    /* test k + 2, guarding against overflow */
    if (!twin && k <= UINT64_MAX - 2 && n % (k + 2) == 0)
        twin = true;
    if (!twin)
        (*count)++;
}

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
