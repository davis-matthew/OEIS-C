/*
 * A319916  Number of integer partitions of any number from 1 to n whose
 *          product of parts is n.
 * https://oeis.org/A319916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{multiplicative partitions of n into integers >1} (n
 *            - sum_of_parts + 1)
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper function: recursively enumerate unordered factorizations
   of 'rem', each factor >= min_factor, accumulating the sum of
   selected parts in 'sum', and adding contributions to *total.  */
static void dfs(idx_t rem, idx_t min_factor, idx_t sum,
                seq_t *total, idx_t n_global)
{
    if (rem == 1) {
        if (sum <= n_global) {
            *total += (seq_t)(n_global - sum + 1);
        }
        return;
    }

    /* try all possible next factors f >= min_factor up to sqrt(rem) */
    for (idx_t f = min_factor; f <= rem / f; ++f) {
        if (rem % f == 0) {
            dfs(rem / f, f, sum + f, total, n_global);
        }
    }

    /* factor 'rem' itself (if valid) as the last part */
    if (rem >= min_factor) {
        dfs(1, rem, sum + rem, total, n_global);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t total = 0;
    dfs(n, 2, 0, &total, n);
    return total;
}
