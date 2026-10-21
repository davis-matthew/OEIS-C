/*
 * A284127  Hosoya triangle of Pell type, read by rows.
 * https://oeis.org/A284127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = P(k) * P(r - k + 1) * where r = smallest integer with
 *            r*(r+1)/2 >= n (row number) * k = n - r*(r-1)/2 (position in the
 *            row, 1‑based) * Pell numbers P are given by * P(0) = 0, P(1) = 1
 *            * P(m) = 2*P(m-1) + P(m-2) for m >= 2 * Offset: a(1) = 1
 * Range:     u8 n <= 28, u16 n <= 91, u32 n <= 351, u64 n <= 1326,
 *            u128 n <= 5151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for Pell numbers. */
static seq_t *pell = 0;          /* pointer to the cache array */
static size_t pell_len = 0;      /* number of Pell numbers stored */

/* Ensure the cache contains Pell numbers up to index n (inclusive). */
static void
ensure_pell(idx_t n)
{
    if ((size_t)n + 1 <= pell_len)
        return;   /* already present */

    size_t new_len = (size_t)n + 1;     /* new required length */

    seq_t *new_pell;
    if (pell == 0) {
        new_pell = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_pell == 0) return;     /* allocation failed */

        new_pell[0] = (seq_t)0;
        if (new_len > 1)
            new_pell[1] = (seq_t)1;
    } else {
        new_pell = (seq_t *)realloc(pell, new_len * sizeof(seq_t));
        if (new_pell == 0) return;     /* allocation failed */
    }

    /* Compute the new Pell numbers */
    for (size_t i = pell_len; i <= (size_t)n; ++i) {
        if (i < 2) continue;           /* already set */
        new_pell[i] = (seq_t)2 * new_pell[i-1] + new_pell[i-2];
    }

    pell = new_pell;
    pell_len = new_len;
}

/* Find the row r such that r*(r+1)/2 >= n.  Uses binary search. */
static inline idx_t
row_of(idx_t n)
{
    idx_t low = 1;
    idx_t high = 1;

    /* Increase high until the condition holds */
    while (((__int128)high * (high + 1)) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main function requested by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, but safe */

    idx_t r = row_of(n);
    idx_t before = (idx_t)(((__int128)(r-1) * r) / 2);  /* triangles count before row r */
    idx_t k = n - before;                 /* position in row r, 1 .. r */

    idx_t idx1 = k;                       /* P(k)        */
    idx_t idx2 = r - k + 1;               /* P(r-k+1)    */

    /* Fill cache to include both required Pell indices. */
    if (idx1 > idx2)
        ensure_pell(idx1);
    else
        ensure_pell(idx2);

    return pell[idx1] * pell[idx2];
}
