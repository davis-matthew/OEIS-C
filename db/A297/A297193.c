/*
 * A297193  Irregular triangle read by rows: rows are partial alternating sums
 *          of rows of A297191.
 * https://oeis.org/A297193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = element of the irregular triangle read by rows. *
 *            Triangle rows are defined by * R[0] = [1]; * R[1] = [1,4,1]; *
 *            For k ≥ 2: R[k] = conv(R[k‑1], [1,4,1]) – place(R[k‑2], 2), *
 *            i.e. subtract row R[k‑2] starting at index 2 of the convolution.
 *            * The sequence is read by rows; n maps to row k =...
 * Range:     u8 n <= 18, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* integer square‑root for 64‑bit indices */
static idx_t floor_sqrt(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32);  /* ≥ sqrt(2^64‑1) */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return lo - 1;
}

/* convolution of a[0..na-1] and b[0..nb-1] into out[0..na+nb-2] */
static void conv(const seq_t *a, idx_t na, const seq_t *b, idx_t nb, seq_t *out)
{
    for (idx_t i = 0; i < na; ++i)
    {
        seq_t ai = a[i];
        for (idx_t j = 0; j < nb; ++j)
            out[i + j] += ai * b[j];
    }
}

/* allocate and zero a row of length len */
static seq_t *new_row(idx_t len)
{
#ifdef __cplusplus
    return new seq_t[len]();
#else
    seq_t *p = (seq_t *)malloc(len * sizeof(seq_t));
    if (!p) abort();
    for (idx_t i = 0; i < len; ++i) p[i] = (seq_t)0;
    return p;
#endif
}

/* base sequence [1,4,1] */
static seq_t base[3] = { (seq_t)1, (seq_t)4, (seq_t)1 };

/* storage for already computed rows */
static seq_t **rows = NULL;
static size_t rows_capacity = 0;
static idx_t rows_used = 0;

/* ensure rows 0..k are computed and stored */
static void ensure_rows(idx_t k)
{
    if (rows_used > k) return;

    if (rows_used + 1 > rows_capacity)
    {
        size_t new_cap = rows_capacity ? rows_capacity * 2 : 8;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) abort();
        rows = tmp;
        rows_capacity = new_cap;
    }

    for (; rows_used <= k; ++rows_used)
    {
        idx_t cur = rows_used;
        idx_t len = 2 * cur + 1;
        seq_t *row = new_row(len);

        if (cur == 0)
        {
            row[0] = (seq_t)1;
        }
        else if (cur == 1)
        {
            conv(rows[0], 1, base, 3, row);
        }
        else
        {
            conv(rows[cur - 1], 2 * (cur - 1) + 1, base, 3, row);
            idx_t start = 2;
            idx_t len_prev = 2 * (cur - 2) + 1;
            for (idx_t p = 0; p < len_prev; ++p)
                row[start + p] -= rows[cur - 2][p];
        }
        rows[cur] = row;
    }
}

seq_t a(idx_t n)
{
    idx_t k = floor_sqrt(n);
    idx_t j = n - k * k;
    ensure_rows(k);
    return rows[k][j];
}
