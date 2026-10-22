/*
 * A378290  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,0) = 0^n and T(n,k) = k * Sum_{r=0..n}
 *          binomial(n+2*r+k,r) * binomial(r,n-r)/(n+2*r+k) for k > 0.
 * https://oeis.org/A378290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(n,k) along anti‑diagonals.
 *            The array satisfies T(n,k) = T(n,k-1) + T(n-1,k+2) + T(n-2,k+3)
 *            for n>1, k>0 with base values T(0,k) = 1 (for all k) T(1,k) = k
 *            (k≥0) T(n,0) = 0 (n>1)
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoised storage of T(n,k) per row. */
struct Row {
    seq_t *data;   /* computed values for this row */
    bool  *good;   /* flag whether data[k] is valid */
    size_t size;   /* current capacity of data/good arrays */
};

/* Global array of rows. */
static struct Row *rows = NULL;
static size_t num_rows = 0;

/* Ensure `rows[n]` exists. */
static void ensure_row(idx_t n)
{
    if ((size_t)n >= num_rows) {
        size_t new_num = (size_t)n + 1;
        rows = (struct Row*)realloc(rows, new_num * sizeof(struct Row));
        for (size_t i = num_rows; i < new_num; ++i) {
            rows[i].data = NULL;
            rows[i].good = NULL;
            rows[i].size = 0;
        }
        num_rows = new_num;
    }
}

/* Ensure capacity for column `k` in row `n`. */
static void ensure_cell(idx_t n, idx_t k)
{
    ensure_row(n);
    if (rows[n].size <= (size_t)k) {
        size_t new_size = (size_t)k + 1;
        rows[n].data = (seq_t*)realloc(rows[n].data,
                                        new_size * sizeof(seq_t));
        rows[n].good = (bool*)realloc(rows[n].good,
                                      new_size * sizeof(bool));
        if (!rows[n].data || !rows[n].good) abort(); /* allocation fail */
        for (size_t i = rows[n].size; i < new_size; ++i)
            rows[n].good[i] = false;
        rows[n].size = new_size;
    }
}

/* Recursive evaluation of T(n,k) with memoisation. */
static seq_t get_T(idx_t n, idx_t k)
{
    /* Base rows. */
    if (n == 0) return (seq_t)1;          /* 0^0 = 1, row 0 all ones */
    if (n == 1) return (seq_t)k;           /* row 1: 0,1,2,... */
    if (k == 0) return (seq_t)0;           /* T(n>1,0) = 0 */

    /* For n>1 and k>0 use the recurrence. */
    ensure_cell(n, k);
    if (rows[n].good[(size_t)k]) return rows[n].data[(size_t)k];

    seq_t val = 0;
    val += get_T(n,    k-1);   /* same row, previous column     */
    val += get_T(n-1,  k+2);   /* previous row, shifted right   */
    val += get_T(n-2,  k+3);   /* two rows above, further right */

    rows[n].data[(size_t)k] = val;
    rows[n].good[(size_t)k] = true;
    return val;
}

/* Given the global index `n`, find the corresponding anti‑diagonal
   coordinates (i,j) of T(i,j).  The calculation uses only 64‑bit
   arithmetic which suffices for the required range. */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find d such that d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid * (mid + 1)) / 2 > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low - 1;
    idx_t base = (d * (d + 1)) / 2;
    *i = n - base;          /* i ranges 0..d */
    *j = d - *i;            /* j = d - i */
}

/* Public sequence entry. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return get_T(i, j);
}
