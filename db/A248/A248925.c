/*
 * A248925  Triangle in which row n consists of the coefficients in
 *          Sum_{m=0..n} x^m * Product_{k=m+1..n} (1-k*x), as read by rows.
 * https://oeis.org/A248925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the term read by rows from the triangle defined by
 *            T(0,0)=1; for k>0, T(r,k)=T(r-1,k)-r*T(r-1,k-1) with T(r,0)=1.
 *            The sequence a(n) lists the entries a(0)=1, a(1)=1, a(2)=0,
 *            a(3)=1, a(4)=-2, a(5)=1, a(6)=1, a(7)=-5, …
 * Range:     i8 n <= 17, i16 n <= 40, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state that is built incrementally between calls */
    static seq_t *cache = NULL;     /* array of all computed a(i) */
    static idx_t cache_size = 0;    /* number of elements in cache */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    static seq_t *prev_row = NULL;  /* array storing the previous row */
    static idx_t prev_row_len = 0;  /* length of the previous row ( == row_number+1 ) */

    /* If the requested index is beyond what has been precomputed,
       extend the cache and the simple triangular table.              */
    if (cache_size <= n) {
        /* First call: initialise the first row and the cache. */
        if (cache_size == 0) {
            cache_cap   = 16;
            cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
            cache[0]    = 1;
            cache_size  = 1;

            prev_row_len = 1;
            prev_row     = (seq_t*)malloc(sizeof(seq_t));
            prev_row[0]  = 1;
        }

        /* Generate subsequent rows until we have a(n) stored. */
        while (cache_size <= n) {
            /* The next row has one more element than the previous row. */
            idx_t new_len = prev_row_len + 1;      /* length of the new row */
            idx_t N       = new_len - 1;           /* number of the new row (0‑based) */

            /* Ensure enough space in the cache for the new entries. */
            if (cache_size + new_len > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap < (size_t)(cache_size + new_len)) new_cap *= 2;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }

            /* Compute the new row from the previous row. */
            seq_t *new_row = (seq_t*)malloc(sizeof(seq_t) * new_len);
            new_row[0] = 1;
            for (idx_t k = 1; k < new_len; ++k) {
                if (k == new_len - 1) {  /* diagonal element */
                    new_row[k] = 1 - ((seq_t)N) * prev_row[k - 1];
                } else {
                    new_row[k] = prev_row[k] - ((seq_t)N) * prev_row[k - 1];
                }
            }

            /* Append the new row to the cache. */
            for (idx_t i = 0; i < new_len; ++i) {
                cache[cache_size + i] = new_row[i];
            }
            cache_size += new_len;

            /* Prepare for the next iteration: free the old row and
               update the previous-row pointer and its length. */
            free(prev_row);
            prev_row     = new_row;
            prev_row_len = new_len;
        }
    }

    return cache[n];
}
