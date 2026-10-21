/*
 * A291700  Number of ways of writing n as a sum of n nonnegative cubes.
 * https://oeis.org/A291700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (Sum_{k>=0} x^(k^3))^n It counts the number of
 *            ordered n‑tuples of non‑negative integers whose cubes sum to n
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 53, u128 n <= 103,
 *            big any n
 * Helpers:   binom, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"
#include "../utils/binom.h"

/* Recursive enumeration of solutions to Σ m_k·k^3 = n,
   where k ≥ 2.  For each solution we multiply the multinomial
   coefficient corresponding to the distribution of the cubes
   among the n positions. */
static void dfs_rec(idx_t idx, idx_t rem_sum, idx_t rem_pos,
                    const idx_t *cubes, idx_t cubes_len,
                    seq_t accum, seq_t *total)
{
    if (idx == cubes_len) {
        /* Remaining sum must be formed with 1^3 = 1.  Let m1 = rem_sum.
           Check that the available positions are enough. */
        idx_t m1 = rem_sum;
        if (m1 > rem_pos)
            return;

        seq_t val = accum;
        if (m1 > 0)
            val *= binom(rem_pos, m1);    /* choose positions for 1's */

        *total += val;                    /* zeros get the remaining slots automatically */
        return;
    }

    idx_t c = cubes[idx];
    idx_t max_by_sum = rem_sum / c;
    idx_t max_by_pos = rem_pos;
    idx_t m_limit = (max_by_sum < max_by_pos) ? max_by_sum : max_by_pos;

    for (idx_t m = 0; m <= m_limit; ++m) {
        seq_t new_acc = accum;
        if (m > 0)
            new_acc *= binom(rem_pos, m);          /* choose positions for this cube */

        dfs_rec(idx + 1,
                rem_sum - m * c,
                rem_pos - m,
                cubes, cubes_len,
                new_acc, total);
    }
}

seq_t a(idx_t n)
{
    /* Build array of cube values k^3 for k = 2 .. floor(n^(1/3)) */
    idx_t maxk = int_cube_root(n);
    idx_t cubes_len = (maxk >= 2) ? (maxk - 1) : 0;

    idx_t *cubes = NULL;
    if (cubes_len > 0) {
        cubes = (idx_t *)malloc(cubes_len * sizeof(idx_t));
        for (idx_t i = 0; i < cubes_len; ++i) {
            idx_t k = i + 2;                     /* start from 2 */
            cubes[i] = k * k * k;                /* k^3 fits in idx_t for given n */
        }
    }

    seq_t total = 0;
    dfs_rec(0, n, n, cubes, cubes_len, (seq_t)1, &total);

    if (cubes)
        free(cubes);

    return total;
}
