/*
 * A282988  Triangle of partitions of an n-set into boxes of size >= m.
 * https://oeis.org/A282988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = Sum_{i=0..n-m} C(n-1, i+m-1) * T(n-i-m, m) * with
 *            initial values T(0,m) = 1 for all m ≥ 1. * The sequence a(n)
 *            enumerates the triangle row‑wise starting from * row 1, column
 *            1. That is, * * a(1) = T(1,1) * a(2) = T(2,1), a(3) = T(2,2) *
 *            a(4) = T(3,1), a(5) = T(3,2), a(6) = T(3,3) * … * *...
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 120, u64 n <= 325,
 *            u128 n <= 903, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

static seq_t *tri   = NULL;   /* 2‑D array rows[0..maxR][0..maxR] (flattened) */
static idx_t  maxR  = 0;      /* highest row currently stored */
static idx_t  stride = 0;     /* current array stride (maxR+1) */

/* Ensure that rows up to R are available; grows the array if necessary. */
static void
ensure_triangle(idx_t R)
{
    if (R <= maxR) return; /* already ready */

    /* size of new square array */
    idx_t new_stride = R + 1;
    size_t new_size  = (size_t)new_stride * (size_t)new_stride;

    seq_t *new_tri = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_tri) abort();          /* out of memory */

    /* zero‑initialise all new entries */
    for (size_t i = 0; i < new_size; ++i) new_tri[i] = (seq_t)0;

    /* copy existing data into the new array */
    if (tri != NULL && maxR > 0) {
        for (idx_t i = 0; i <= maxR; ++i) {
            for (idx_t j = 0; j <= maxR; ++j) {
                new_tri[i * new_stride + j] =
                    tri[i * stride + j];
            }
        }
        free(tri);                   /* free old storage */
    }

    tri    = new_tri;
    stride = new_stride;

    /* initialise the extra row (row 0) – T(0,m)=1 for all m≥1 */
    for (idx_t m = 1; m <= R; ++m) tri[0 * stride + m] = (seq_t)1;

    /* Compute any new rows from (maxR+1) up to R */
    for (idx_t n = maxR + 1; n <= R; ++n) {
        for (idx_t m = 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i <= n - m; ++i) {
                seq_t coeff = binom(n - 1, i + m - 1);
                seq_t val   = tri[(n - i - m) * stride + m];
                sum += coeff * val;
            }
            tri[n * stride + m] = sum;
        }
    }

    maxR = R;
}

/* Map a linear index n (1‑based) to its (row, column) in the triangle.
 * The triangle is 1‑based: rows and columns start at 1; row 0 is not
 * part of the enumeration (it only holds the T(0,m) initial values). */
static void
index_to_rc(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    /* find minimal r with r*(r+1)/2 >= n (rows start at 1) */
    while (r * (r + 1) / 2 < n) ++r;  /* r will be >=1 for n>=1 */
    *row = r;
    *col = n - r * (r - 1) / 2;
}

/* Public function required by the framework.  It returns a(n) from
 * the OEIS triangle A282988.  It is memoised so that arbitrary
 * order of calls works fine. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* offset is 1; n=0 unused */
    idx_t r, c;
    index_to_rc(n, &r, &c);
    ensure_triangle(r);
    return tri[r * stride + c];
}
