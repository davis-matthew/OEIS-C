/*
 * A352211  Largest number of maximal node-induced cluster subgraphs of an
 *          n-node graph.
 * https://oeis.org/A352211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 3, a(4) = 6, a(5) = 8, a(6) = 11 For
 *            n ≥ 7: a(n) = 6 * a(n−4) (log‑superadditive recurrence)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t base[6] = {1, 1, 3, 6, 8, 11};

    if (n <= 6)
        return base[n - 1];

    /* Number of times the recurrence is applied */
    idx_t k = (n - 3) / 4;                     /* k ≥ 1 for n ≥ 7 */

    /* Index of the base case after k applications */
    idx_t base_index = n - 4 * k;              /* 1 ≤ base_index ≤ 6 */

    seq_t result = base[base_index - 1];

    /* Multiply by 6 repeatedly */
    for (idx_t i = 0; i < k; ++i)
        result *= (seq_t)6;

    return result;
}
