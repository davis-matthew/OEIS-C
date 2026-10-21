/*
 * A245842  Triangle T read by rows: T(n,k) = Total number of even parts in
 *          all partitions of n with exactly k parts, 1 <= k <= n.
 * https://oeis.org/A245842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = total number of even parts in all partitions of the
 *            integer * that appears at position n when the triangle T read by
 *            rows is flattened. * For a given index n, let r be the smallest
 *            integer with r*(r+1)/2 >= n, * and let k = n - (r-1)*r/2. Then
 *            a(n) = T(r, k), where * T(r,k) is the total nu...
 * Range:     u8 n <= 215, u16 n <= 1186, u32 n <= 9067, u64 n <= 7831,
 *            u128 n <= 7831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t       cap   = 0;          /* maximum n for which tables are built */
static seq_t *     p_tbl = NULL;       /* partition counts */
static seq_t *     e_tbl = NULL;       /* sum of even parts */

/* recompute entire tables up to max_n (inclusive) */
static void recompute_tables(idx_t max_n)
{
    size_t total = (size_t)(max_n + 1) * (max_n + 1);
    /* ensure storage */
    free(p_tbl);
    free(e_tbl);
    p_tbl = (seq_t *)calloc(total, sizeof(seq_t));
    e_tbl = (seq_t *)calloc(total, sizeof(seq_t));
    if (!p_tbl || !e_tbl) return;           /* allocation failed – caller will crash */

    cap = max_n;

    /* base row 0 */
    p_tbl[0] = (seq_t)1;             /* p[0][0] = 1 */
    /* e[0][0] is already zeroed */

    /* DP */
    for (idx_t n = 1; n <= max_n; ++n)
    {
        for (idx_t k = 1; k <= n; ++k)
        {
            /* p[n][k] */
            seq_t p_val = p_tbl[(n - 1) * (max_n + 1) + (k - 1)];
            if (n >= 2 * k)
                p_val += p_tbl[(n - k) * (max_n + 1) + k];

            /* e[n][k] */
            seq_t e_val = e_tbl[(n - 1) * (max_n + 1) + (k - 1)];
            if (n >= 2 * k) {
                seq_t p_sub = p_tbl[(n - k) * (max_n + 1) + k];
                seq_t e_sub = e_tbl[(n - k) * (max_n + 1) + k];
                seq_t o_sub = (seq_t)k * p_sub - e_sub;   /* odd parts of the transformed partition */
                e_val += o_sub;
            }

            p_tbl[n * (max_n + 1) + k] = p_val;
            e_tbl[n * (max_n + 1) + k] = e_val;
        }
    }
}

/* ensure tables are at least up to needed n */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cap) return;
    recompute_tables(needed);
}

/* binary search for smallest r with r*(r+1)/2 >= n */
static idx_t row_from_index(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1, but guard against 0 */

    idx_t r = row_from_index(n);
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;       /* 1 <= k <= r */

    ensure_capacity(r);

    return e_tbl[r * (cap + 1) + k];
}
