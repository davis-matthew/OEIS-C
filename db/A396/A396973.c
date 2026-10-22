/*
 * A396973  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B^k(x), where
 *          B(x) is the e.g.f. of A395269.
 * https://oeis.org/A396973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n,k,5) satisfies * a(1,k) = 1 for all k * a(n,0) = 0 for n > 1
 *            * a(n,k) = a(n,k-1) + Σ_{j=1}^{n-1} * binomial(n-1,j) × a(j,k+4)
 *            × a(n-j,k-1) for n > 1, k > 0 * The required sequence a(n) is
 *            obtained by flattening this square * array along antidiagonals
 *            (row‑first)
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 63, u64 n <= 133,
 *            u128 n <= 298, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A396973 (red from the e.g.f. A395269 with l = 5). */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* function: seq_t binomial(idx_t a, idx_t b) */

static size_t g_rows = 0;      /* maximum n stored (0‑based: g_rows >= needed n)       */
static size_t g_cols = 0;      /* maximum k stored; columns 0 .. g_cols inclusive     */
static seq_t *g_data  = NULL;  /* flattened table of a values                        */
static unsigned char *g_vis = NULL;   /* visited flag for each cell 0/1                    */

/* Ensure the internal tables can hold indices up to (neededR, neededK). */
static void ensure_capacity(idx_t neededR, idx_t neededK)
{
    size_t r = (size_t)neededR;
    size_t k = (size_t)neededK;

    if (r <= g_rows && k <= g_cols) return;   /* already big enough */

    size_t newRows = r;
    size_t newCols = k;
    size_t newSize = (newRows + 1) * (newCols + 1);

    seq_t  *newData = (seq_t *)calloc(newSize, sizeof(seq_t));
    unsigned char *newVis = (unsigned char *)calloc(newSize, 1);

    if (!newData || !newVis) abort();   /* memory allocation failed */

    /* copy old data */
    if (g_data) {
        for (size_t row = 0; row <= g_rows; ++row) {
            size_t oldOff = row * (g_cols + 1);
            size_t newOff = row * (newCols + 1);
            size_t copyLen = g_cols + 1;
            if (copyLen > newCols + 1) copyLen = newCols + 1;
            memcpy(newData + newOff, g_data + oldOff, copyLen * sizeof(seq_t));
            memcpy(newVis + newOff, g_vis  + oldOff, copyLen);
        }
        free(g_data);
        free(g_vis);
    }

    g_data = newData;
    g_vis  = newVis;
    g_rows = newRows;
    g_cols = newCols;
}

/* Flattened array index calculation */
static inline size_t idx_off(idx_t n, idx_t k)
{
    return (size_t)n * (g_cols + 1) + (size_t)k;
}

/* Recursive evaluation of a(n,k,5) with memoisation. */
static seq_t rec_a(idx_t n, idx_t k)
{
    if (n == 1) return (seq_t)1;          /* a(1,k) = 1   */
    if (k == 0) return (seq_t)0;          /* a(n>1,0) = 0 */

    size_t off = idx_off(n, k);
    if (g_vis[off]) return g_data[off];

    seq_t sum = rec_a(n, k - 1);          /* a(n,k-1) */

    for (idx_t j = 1; j < n; ++j) {
        seq_t bin  = binomial(n - 1, j);
        seq_t part1 = rec_a(j, k + 4);    /* a(j,k+4)   */
        seq_t part2 = rec_a(n - j, k - 1);// a(n-j,k-1)
        sum += (part1 * part2 * bin);
    }

    g_data[off] = sum;
    g_vis[off]  = 1;
    return sum;
}

/* Flattening of the square array along antidiagonals (row‑first). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, so n>=1 is used */

    /* Determine diagonal s such that s(s+1)/2 >= n */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n) ++s;

    /* Compute (row, col) from index n */
    idx_t start  = (s - 1) * s / 2 + 1;                 /* first index on this diagonal */
    idx_t offset = n - start;                          /* 0‑based offset within diagonal */
    idx_t row    = offset + 1;                          /* 1‑based row number */
    idx_t col    = s - row;                             /* column (k) */

    /* Required table size:
     *   rows   up to row
     *   columns up to 5*(row-1)  (proved to be enough for all recursions) */
    idx_t neededR = row;
    idx_t neededK = 5 * (row - 1);

    ensure_capacity(neededR, neededK);
    return rec_a(row, col);
}
