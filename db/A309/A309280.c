/*
 * A309280  T(n,k) is (1/k) times the sum of the elements of all subsets of
 *          [n] whose sum is divisible by k; triangle T(n,k), n >= 1, 1 <= k
 *          <= n*(n+1)/2, read by rows.
 * https://oeis.org/A309280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the flattened triangle T(n,k) where * T(n,k) = (1/k) ·
 *            sum_{ S ⊆ [1..n], sum(S) ≡ 0 (mod k) } sum(S)
 * Range:     u8 n <= 35, u16 n <= 220, u32 n <= 2600, u64 n <= 9073,
 *            u128 n <= 9073, big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/free_seq_arr.h"
 
/* Helper: allocate an array of seq_t of given size, construct it in C++,
 * or just calloc in C.  Must be freed by matching free_seq. */
static inline seq_t* alloc_seq_arr(size_t sz)
{
#if defined(__cplusplus)
    return new seq_t[sz];
#else
    return (seq_t*)calloc(sz, sizeof(seq_t));
#endif
}

 
/* Compute a single row n of the triangle and store it in *row_out.
 * The row has len = n*(n+1)/2 entries. */
static void compute_row(idx_t n, seq_t **row_out)
{
    /* total possible subset sum */
    size_t maxsum = (size_t)n * (n + 1) / 2;
    seq_t *cnt = alloc_seq_arr(maxsum + 1);   /* number of subsets with a given sum */
    seq_t *sum = alloc_seq_arr(maxsum + 1);   /* total element sum over those subsets */
 
    if (!cnt || !sum) { *row_out = NULL; return; }
    /* empty subset */
    cnt[0] = 1;
 
    /* dynamic programming over the numbers 1..n */
    for (idx_t val = 1; val <= n; ++val) {
        seq_t val_s = (seq_t)val;
        for (size_t s = maxsum; s >= (size_t)val; --s) {
            size_t prev = s - (size_t)val;
            /* new subsets that include val */
            seq_t add_cnt = cnt[prev];
            seq_t add_sum = sum[prev] + val_s * add_cnt;
            cnt[s] += add_cnt;
            sum[s] += add_sum;
        }
    }
 
    /* Build the triangle row from the sums */
    seq_t *row = alloc_seq_arr(maxsum);
    if (!row) { free_seq_arr(cnt); free_seq_arr(sum); *row_out = NULL; return; }
 
    for (size_t k = 1; k <= maxsum; ++k) {
        seq_t total = 0;
        for (size_t m = k; m <= maxsum; m += k)
            total += sum[m];
        row[k-1] = total / k;   /* integer division by k */
    }
 
    free_seq_arr(cnt);
    free_seq_arr(sum);
    *row_out = row;
}
 
/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* before offset */
 
    /* Find the row r such that the flattened index n lies in that row */
    idx_t r = 0;
    unsigned __int128 cumulative = 0;
    while (1) {
        ++r;
        unsigned __int128 row_len = (unsigned __int128)r * (r + 1) / 2;
        unsigned __int128 next = cumulative + row_len;
        if ((unsigned __int128)n <= next) break;
        cumulative = next;
    }
    /* column index (1‑based) within row r */
    idx_t k = (idx_t)((unsigned __int128)n - cumulative);
 
    /* Static cache of already computed rows */
    static seq_t **rows = NULL;          /* array of pointers to rows */
    static idx_t rows_used = 0;          /* number of rows stored (rows[0..used-1]) */
    static size_t rows_cap = 0;          /* allocated capacity of rows */
 
    if (rows_used < r) {
        /* Extend the rows array to hold up to r rows */
        size_t new_cap = r;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp && new_cap > 0) { free(rows); return 0; }
        rows = tmp;
        /* Compute and store the new rows */
        for (idx_t m = rows_used + 1; m <= r; ++m) {
            seq_t *row;
            compute_row(m, &row);
            rows[m-1] = row;     /* store pointer */
        }
        rows_used = r;
    }
 
    seq_t *row = rows[r-1];
    size_t maxsum = (size_t)r * (r + 1) / 2;
    if (k == 0 || k > maxsum) return 0;
    return row[k-1];
}
