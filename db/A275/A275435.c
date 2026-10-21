/*
 * A275435  Triangle read by rows: T(n,k) is the number of 00-avoiding binary
 *          words of length n having degree of asymmetry equal to k (n>=0;
 *          0<=k<=floor(n/2)).
 * https://oeis.org/A275435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   len_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275435: the flat sequence obtained by writing the triangle
   T(n,k), where T(n,k) counts 00‑avoiding binary words of length n
   having degree of asymmetry k, row by row. The triangle satisfies
   the recurrence
        T(n,k) = T(n‑2,k) + T(n‑2,k‑1) + T(n‑4,k) + T(n‑4,k‑1)
                − T(n‑6,k‑1)   (for n ≥ 6, k ≥ 0),
   with all out‑of‑range terms treated as 0.  The first six rows
   are given explicitly.  The function a(idx_t n) returns the n‑th
   element of the flattened triangle.  Memoisation is used so that
   calls to a() are fast even for n≈10000.  No global state other
   than the static tables is modified. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/len_of.h"

static seq_t **rows = NULL;          /* rows[r] points to array of T(r,k) */
static idx_t *starts = NULL;         /* starts[r] = linear index of first element of row r */
static idx_t rows_used = 0;
static idx_t rows_alloc = 0;

seq_t a(idx_t n)
{
    /* Grow tables until they contain the row that holds index n */
    while (rows_used == 0 || starts[rows_used - 1] + len_of(rows_used - 1) <= n)
    {
        /* Allocate space for the next row pointer and its start index */
        if (rows_used == rows_alloc)
        {
            idx_t new_alloc = rows_alloc ? rows_alloc * 2 : 8;
            seq_t **new_rows = (seq_t **)realloc(rows, new_alloc * sizeof(seq_t *));
            if (!new_rows) abort();
            rows = new_rows;

            idx_t *new_start = (idx_t *)realloc(starts, new_alloc * sizeof(idx_t));
            if (!new_start) abort();
            starts = new_start;

            rows_alloc = new_alloc;
        }

        idx_t r = rows_used;
        idx_t len = len_of(r);
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();

        /* ---- Base rows 0…5 ---- */
        if (r == 0)
        {
            row[0] = 1;
        }
        else if (r == 1)
        {
            row[0] = 2;
        }
        else if (r == 2)
        {
            row[0] = 1;
            row[1] = 2;
        }
        else if (r == 3)
        {
            row[0] = 3;
            row[1] = 2;
        }
        else if (r == 4)
        {
            row[0] = 2;
            row[1] = 4;
            row[2] = 2;
        }
        else if (r == 5)
        {
            row[0] = 5;
            row[1] = 6;
            row[2] = 2;
        }
        else
        {
            /* ---- Recurrence for r ≥ 6 ---- */
            idx_t len_r2 = len_of(r - 2);
            idx_t len_r4 = len_of(r - 4);
            idx_t len_r6 = len_of(r - 6);   /* used only when k>0 */

            for (idx_t k = 0; k < len; ++k)
            {
                seq_t v = 0;

                /* contributions from row r-2 */
                if (k < len_r2)          v += rows[r - 2][k];
                if (k > 0 && k - 1 < len_r2) v += rows[r - 2][k - 1];

                /* contributions from row r-4 */
                if (k < len_r4)          v += rows[r - 4][k];
                if (k > 0 && k - 1 < len_r4) v += rows[r - 4][k - 1];

                /* subtraction from row r-6 */
                if (r >= 6 && k > 0 && k - 1 < len_r6)
                    v -= rows[r - 6][k - 1];

                row[k] = v;
            }
        }

        rows[r] = row;
        starts[r] = (r == 0) ? 0 : starts[r - 1] + len_of(r - 1);
        ++rows_used;
    }

    /* Find the row that contains the requested linear index n */
    idx_t lo = 0, hi = rows_used - 1;
    while (lo <= hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t next_start = (mid + 1 < rows_used) ? starts[mid + 1] : SIZE_MAX;

        if (n < starts[mid])
        {
            hi = mid - 1;
        }
        else if (n >= next_start)
        {
            lo = mid + 1;
        }
        else
        {
            return rows[mid][n - starts[mid]];
        }
    }

    /* Should never happen */
    abort();
}
