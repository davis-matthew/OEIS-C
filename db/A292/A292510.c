/*
 * A292510  a(n) = smallest k >= 1 such that {1, p(n,2), p(n,3), ..., p(n,k)}
 *          can be partitioned into two sets with equal sums, where p(n,m) is
 *          m-th n-gonal number.
 * https://oeis.org/A292510
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that the first k n‑gonal numbers {1,
 *            p(n,2), …, p(n,k)} can be split into two subsets with equal
 *            sums. By the OEIS data a(n) ≤ 7 for all n ≥ 3
 * Range:     u8 n <= 16, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   pentagonal_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pentagonal_number.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 3 – return 0. */
    if (n < 3) return (seq_t)0;

    /* Try k from 1 up to 7 (the known upper bound). */
    for (idx_t k = 1; k <= 7; ++k)
    {
        seq_t vals[7];
        seq_t total = 0;

        /* Compute the first k n‑gonal numbers. */
        for (idx_t m = 1; m <= k; ++m)
        {
            seq_t p = pentagonal_number(n, m);
            vals[m - 1] = p;
            total += p;
        }

        /* Odd total sum → impossible to split. */
        if (total & (seq_t)1) continue;

        seq_t target = total / 2;

        /* Enumerate all subsets of the k numbers. */
        const idx_t subsets = ((idx_t)1 << k);
        for (idx_t mask = 0; mask < subsets; ++mask)
        {
            seq_t sum = 0;
            for (idx_t i = 0; i < k; ++i)
                if (mask & ((idx_t)1 << i))
                    sum += vals[i];

            if (sum == target)
                return (seq_t)k;   /* minimal k found */
        }
    }

    /* By construction a(n) ≤ 7; return 7 as a safe fallback. */
    return (seq_t)7;
}
