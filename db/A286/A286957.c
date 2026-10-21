/*
 * A286957  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} (1 + k*x^j).
 * https://oeis.org/A286957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 314, u32 n <= 1302, u64 n <= 6632,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A(i,k) where A(i,k) is the number of subsets of
 * {1,2,3,…} that sum to i, each chosen subset contributing k^|subset|
 * (so the column generating function is ∏_{j≥1}(1 + k x^j)).
 * The sequence a(n) is read by antidiagonals: i + k = d where d =
 * floor((√(8n+1)-1)/2).
 */

#include <stdlib.h>

/* public interface ---------------------------------------------------- */
seq_t a(idx_t n);

/* internal static data ------------------------------------------------ */
static seq_t *dp_table = NULL;   /* dp_table[sum][t]  (row-major) */
static idx_t  max_sum   = 0;     /* size of the table (max sum processed) */

/* --------------------------------------------------------------------- */
static inline idx_t antidiag(idx_t n)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    return d;
}

/* rebuild the DP table up to new_size (inclusive) */
static void recompute_dp(idx_t new_size)
{
    /* free old table if present */
    if (dp_table) free(dp_table);

    /* allocate fresh table: (new_size+1) × (new_size+1) entries */
    size_t stride   = (size_t)new_size + 1;
    size_t total    = stride * stride;
    dp_table = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp_table) return;          /* allocation failed: crash on use is OK */

    dp_table[0] = (seq_t)1;         /* dp[0][0] = 1 */

    /* dynamic programming over distinct parts */
    for (idx_t part = 1; part <= new_size; ++part) {
        for (idx_t sum = new_size; sum >= part; --sum) {
            size_t base = (size_t)sum * stride;
            size_t src  = (size_t)(sum - part) * stride;
            for (idx_t t = new_size; t >= 1; --t) {
                dp_table[base + t] += dp_table[src + (t - 1)];
            }
        }
    }
    max_sum = new_size;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* determine coordinates (i,k) of the element at linear index n   */
    idx_t d   = antidiag(n);                      /* antidiagonal number */
    idx_t prev = d * (d + 1) / 2;                 /* elements before diag  */
    idx_t i   = n - prev;                         /* row index on this diag*/
    idx_t k   = d - i;                            /* column index          */

    /* ensure the DP table covers the required size */
    if (dp_table == NULL || d > max_sum)
        recompute_dp(d);

    /* evaluate A(i,k) = Σ_{t=0}^{i} dp[i][t] · k^t  (Horner's rule) */
    seq_t val = 0;
    seq_t base = (seq_t)k;
    size_t stride = (size_t)d + 1;                /* stride of DP table   */
    size_t row   = (size_t)i * stride;            /* offset to row i      */

    for (idx_t t = i; t > 0; --t) {
        val = val * base + dp_table[row + t];
    }
    val = val * base + dp_table[row];             /* add dp[i][0] */

    return val;
}
