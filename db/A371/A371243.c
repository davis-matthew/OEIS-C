/*
 * A371243  Number of factorizations of n into factors > 2.
 * https://oeis.org/A371243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of factorizations of n into factors > 2 a(n) counts
 *            unordered factorizations of n into integers > 2, including the
 *            trivial factorization n itself. The implementation uses a simple
 *            recursive search with a minimal non‑decreasing ordering
 *            constraint to avoid duplicate permutations
 * Range:     u8 n <= 8639, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9612 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t count_factorization(idx_t n, idx_t min_factor)
{
    seq_t total = 0;
    /* trivial factorization: n itself */
    if (n >= min_factor)
        total = 1;

    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t nd = n / d;
            /* ensure non‑decreasing order, avoid symmetric duplicates */
            if (d <= nd)
                total += count_factorization(nd, d);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    /* OEIS offset : a(1)=1, a(2)=0 */
    switch (n) {
    case 1: return 1;
    case 2: return 0;
    default: return count_factorization(n, 3);
    }
}
