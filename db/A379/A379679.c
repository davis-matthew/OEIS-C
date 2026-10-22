/*
 * A379679  Number of finite sets of positive integers > 1 with sum + product
 *          = n.
 * https://oeis.org/A379679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of finite sets of integers >1 with sum of the set
 *            + product of the set = n. (Empty set counted only for n = 1.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper recursive search.  count is accumulated in *cnt. */
static void dfs(idx_t start, seq_t sum, seq_t prod, seq_t n, seq_t *cnt)
{
    for (idx_t x = start; x <= n; ++x) {
        seq_t sx = (seq_t)x;
        if (prod > n / sx)            /* product would exceed n */
            break;

        seq_t new_prod = prod * sx;
        seq_t new_sum  = sum + sx;

        if (new_sum + new_prod > n)
            break;                   /* further x only grow sum+prod */

        if (new_sum + new_prod == n)
            (*cnt)++;                /* found a valid set */

        if (new_sum + new_prod < n)  /* explore further elements */
            dfs(x + 1, new_sum, new_prod, n, cnt);
    }
}

/* Main function as required */
seq_t a(idx_t n)
{
    if (n == 1)          /* empty set only for n = 1 */
        return (seq_t)1;

    seq_t cnt = 0;
    dfs(2, 0, 1, (seq_t)n, &cnt);   /* start with prod=1 (empty product) */
    return cnt;
}
