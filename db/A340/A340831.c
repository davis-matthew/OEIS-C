/*
 * A340831  Number of factorizations of n into factors > 1 with odd greatest
 *          factor.
 * https://oeis.org/A340831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 1
 *            with odd greatest factor. a(n) is 0 for n = 1,2
 * Range:     u8 n <= 9035, u16 n <= 9035, u32 n <= 9035, u64 n <= 9035,
 *            u128 n <= 9035, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: recursive DFS that counts factorizations of rem
   under the constraints:
   - all chosen factors are ≥ min_factor
   - the maximum factor seen so far is stored in max_factor
   - each leaf corresponds to one factorization;
     it is counted only if the final maximum factor is odd. */
static void dfs_factor(idx_t rem, idx_t min_factor, idx_t max_factor, seq_t *cnt)
{
    /* Try all factors f such that rem % f == 0 and min_factor ≤ f ≤ sqrt(rem) */
    for (idx_t f = min_factor; f <= rem / f; ++f)
    {
        if ((rem % f) == 0)
        {
            idx_t new_max = (f > max_factor) ? f : max_factor;
            dfs_factor(rem / f, f, new_max, cnt);
        }
    }

    /* The Remaining factor can be taken alone if it is ≥ min_factor.
       This corresponds to a factorization consisting only of rem itself. */
    if (rem >= min_factor)
    {
        idx_t new_max = (rem > max_factor) ? rem : max_factor;
        if ((new_max & 1u) != 0)   /* only count if greatest factor is odd */
            (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)               /* a(1) = 0, a(2) = 0 */
        return 0;

    seq_t result = 0;
    dfs_factor(n, 2, 0, &result);
    return result;
}
