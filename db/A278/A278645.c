/*
 * A278645  Triangle read by rows in which each new term is the sum of its two
 *          largest neighbors in the structure.
 * https://oeis.org/A278645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   tri_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A278645: 
   T(1,1)=1; for row r≥2, column c (1≤c≤r) let
   L     = T(r,c‑1)            if c>1
   UL    = T(r‑1,c‑1)          if c>1
   UR    = T(r‑1,c)            if c≤r‑1
   ULC   = T(r‑1,c+1)          if c≤r‑2
   Then T(r,c) = sum of the two largest among the existing neighbors
   (i.e. among the set {L,UL,UR,ULC} that are present). 
   The linear sequence a(n) is obtained by reading the triangle by rows. */

#include <stdlib.h>
#include "../utils/tri_count.h"

seq_t a(idx_t n)
{
    /* Flat storage of the triangle, 0‑based */
    static seq_t *tri = NULL;
    static idx_t rows = 0;          /* number of rows already computed */

    /* Find the row containing the n-th term (1‑based row number) */
    idx_t lo = 1, hi = 1;
    while (tri_count(hi) < n) hi <<= 1;
    while (lo < hi)
    {
        idx_t mid = (lo + hi) >> 1;
        if (tri_count(mid) >= n) hi = mid;
        else                      lo = mid + 1;
    }
    idx_t row = lo;       /* row of the n‑th term */

    /* Grow the triangle if necessary */
    if (row > rows)
    {
        idx_t new_rows = row;
        size_t new_total = (size_t)tri_count(new_rows);

        /* Reallocate with an explicit cast to accept void* in C++ */
        seq_t *tmp = (seq_t *)realloc(tri, new_total * sizeof(seq_t));
        if (!tmp) exit(1);   /* out of memory */
        tri = tmp;

        /* Compute new rows incrementally */
        for (idx_t r = rows + 1; r <= new_rows; ++r)
        {
            for (idx_t c = 1; c <= r; ++c)
            {
                /* Index of T(r,c) in the flat array */
                idx_t pos = tri_count(r - 1) + c - 1;

                if (r == 1 && c == 1)
                {
                    tri[pos] = 1;   /* first element */
                    continue;
                }

                seq_t max1 = 0, max2 = 0;

                /* Left neighbor in the same row */
                if (c > 1)
                {
                    seq_t v = tri[pos - 1];
                    if (v > max1) { max2 = max1; max1 = v; }
                    else if (v > max2) max2 = v;
                }
                /* Upper‑left neighbor (r-1, c-1) */
                if (r > 1 && c > 1)
                {
                    seq_t v = tri[tri_count(r - 2) + c - 2];
                    if (v > max1) { max2 = max1; max1 = v; }
                    else if (v > max2) max2 = v;
                }
                /* Upper‑right neighbor (r-1, c) */
                if (r > 1 && c <= r - 1)
                {
                    seq_t v = tri[tri_count(r - 2) + c - 1];
                    if (v > max1) { max2 = max1; max1 = v; }
                    else if (v > max2) max2 = v;
                }
                /* Upper‑right‑extended neighbor (r-1, c+1) */
                if (r > 1 && c <= r - 2)
                {
                    seq_t v = tri[tri_count(r - 2) + c];
                    if (v > max1) { max2 = max1; max1 = v; }
                    else if (v > max2) max2 = v;
                }

                tri[pos] = max1 + max2;
            }
        }

        rows = new_rows;
    }

    /* Return the n‑th term */
    idx_t offset = tri_count(row - 1);
    return tri[offset + (n - offset - 1)];
}
