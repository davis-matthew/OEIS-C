/*
 * A305253  Number of connected factorizations of n into factors greater than
 *          1 whose distinct factors are pairwise indivisible.
 * https://oeis.org/A305253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of connected factorizations of n into factors >1 *
 *            whose distinct factors are pairwise indivisible. The sequence is
 *            computed * by enumerating all unordered factorizations of n and
 *            keeping those that * satisfy the connectivity and indivisibility
 *            conditions
 * Range:     u8 n <= 9343, u16 n <= 9343, u32 n <= 9343, u64 n <= 9343,
 *            u128 n <= 9343, big any n
 * Helpers:   _dfs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_dfs.h"

/* Sequence function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0ULL)          /* offset starts at 1; undefined for 0 */
        return (seq_t)0;

    /* static cache for n <= 10000 */
    static seq_t  cache[10001];
    static bool   computed[10001] = {false};

    if (n <= 10000ULL)
    {
        if (computed[n])
            return cache[n];
    }

    seq_t result = 0;
    uint64_t curr[20];      /* max depth needed for n <= 10000 is < 20 */

    _dfs((uint64_t)n, 2, curr, 0, &result);

    if (n <= 10000ULL)
    {
        cache[n] = result;
        computed[n] = true;
    }
    return result;
}
