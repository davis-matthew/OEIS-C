/*
 * A249248  Triangular array: row n gives the coefficients of the polynomial
 *          p(n,x) defined in Comments.
 * https://oeis.org/A249248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 160, u64 n <= 449,
 *            u128 n <= 1350, big any n
 * Validated: standard, pass, 74 terms through n = 4459 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A249248: Coefficients of the numerator polynomial p(n,x)
   defined by:
        f(1,x) = 1,
        f(n,x) = (x + 1) + n*x/f(n-1,x)   (n ≥ 2)
   Let p(n,x) be the numerator of f(n,x) = p(n,x)/p(n-1,x).
   Recurrence for n ≥ 3:
        p(1,x) = 1
        p(2,x) = 1 + 3x
        p(n,x) = (x + 1)*p(n-1,x) + n*x*p(n-2,x)

   The sequence a(n) lists those coefficients row by row:
        row n gives the coefficients of p(n,x) (degree n-1).
        a(1)=1  (row 1)
        a(2)=1  (row 2, coeff x⁰)
        a(3)=3  (row 2, coeff x¹)
        a(4)=1  (row 3, coeff x⁰)
        a(5)=12 (row 3, coeff x¹)
        ...
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return the (row, position) of the coefficient corresponding
   to overall index n (1‑based).  Positions are 0‑based within the row. */
static void get_row_pos(idx_t n, idx_t *row, idx_t *pos)
{
    /* Find smallest r such that r*(r+1)/2 ≥ n.  Use a simple loop;
       for n ≤ 10000 this is trivial. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Number of elements in all preceding rows. */
    idx_t prev_sum = (r - 1) * r / 2;
    *row = r;
    *pos = n - prev_sum - 1;
}

/* Forward declaration of seq_t and idx_t */
extern seq_t _dummy_seq_t;
extern idx_t _dummy_idx_t;

/* Static cache of all rows that have been computed.
   row_cache[i] points to an array of length i containing the
   coefficients of p(i,x).  Index 0 is unused (rows are 1‑indexed). */
static seq_t **row_cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity for row_cache */
static idx_t max_cached_row = 0;  /* largest row that has been computed */

/* Ensure all rows up to target_row are computed and stored in row_cache. */
static void ensure_rows(idx_t target_row)
{
    if (target_row <= max_cached_row)
        return;

    /* Increase the cache if necessary. */
    if (target_row + 1 > cache_cap) {
        size_t new_cap = target_row + 1;
        seq_t **new_cache = (seq_t **)realloc(row_cache,
                                              new_cap * sizeof(seq_t *));
        if (!new_cache)
            exit(1);
        for (idx_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = NULL;
        row_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute rows incrementally. */
    for (idx_t i = max_cached_row + 1; i <= target_row; ++i) {
        seq_t *row = mem_new(seq_t, i);
        if (!row)
            exit(1);

        if (i == 1) {            /* p(1,x) = 1 */
            row[0] = (seq_t)1;
        } else if (i == 2) {     /* p(2,x) = 1 + 3x */
            row[0] = (seq_t)1;
            row[1] = (seq_t)3;
        } else {
            seq_t *prev  = row_cache[i - 1];   /* p(i-1,x)  */
            seq_t *prev2 = row_cache[i - 2];   /* p(i-2,x)  */

            /* Constant term comes solely from (x+1)*prev. */
            row[0] = prev[0];

            /* Coefficients for x^k, k = 1 .. i-1. */
            for (idx_t k = 1; k < i; ++k) {
                seq_t val = 0;

                /* Term from prev[k] due to the +1 part of (x+1). */
                if (k <= i - 2)   /* prev has length i-1 */
                    val += prev[k];

                /* Term from x*prev[k-1] due to the 'x' part. */
                val += prev[k - 1];

                /* Term from n*x*prev2[k-1], present only when k-1 < i-2. */
                if (k <= i - 2)
                    val += (seq_t)i * prev2[k - 1];

                row[k] = val;
            }
        }

        row_cache[i] = row;
    }

    max_cached_row = target_row;
}

/* The sequence function requested by the build. */
seq_t a(idx_t n)
{
    idx_t r, pos;
    get_row_pos(n, &r, &pos);

    /* Compute rows up to required r if not already cached. */
    ensure_rows(r);

    /* Return the coefficient at position pos in row r. */
    return row_cache[r][pos];
}
