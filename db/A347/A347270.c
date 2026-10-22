/*
 * A347270  Square array T(n,k) in which row n lists the 3x+1 sequence
 *          starting at n, read by antidiagonals upwards, with n >= 1 and k >=
 *          0.
 * https://oeis.org/A347270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 710, u16 n <= 8286, u32 n <= 8286, u64 n <= 8286,
 *            u128 n <= 8286, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347270:
   a(n) enumerates the infinite square array T(n,k) where row n contains the
   Collatz (3x+1) sequence starting at n, read by upward antidiagonals.
   The code builds the array incrementally, caches all values, and returns
   the n‑th term (n ≥ 1).  It works for any integer type seq_t and any
   unsigned integer type idx_t. */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* cache of all computed sequence values  */
    static seq_t *seq_cache = NULL;
    static size_t seq_cache_size = 0;
    static size_t seq_cache_cap = 0;

    /* current Collatz values for each row, indexed by n (1‑based)  */
    static seq_t *row_vals = NULL;
    static size_t row_cap = 0;

    /* the index of the next diagonal to generate   */
    static idx_t cur_diagonal = 0;

    if (n == 0)                /* defined only for n ≥ 1 */
        return (seq_t)0;

    /* generate diagonals until the cache holds at least n elements */
    while (seq_cache_size < (size_t)n) {
        /* ensure row_vals can store indices up to cur_diagonal+1  */
        if (row_cap <= (size_t)cur_diagonal + 1) {
            size_t needed = (size_t)cur_diagonal + 2;   /* +1 for 1‑based index */
            size_t new_cap = row_cap ? row_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(row_vals, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            row_vals = tmp;
            row_cap = new_cap;
        }

        /* process diagonal cur_diagonal   (n + k = cur_diagonal + 1) */
        for (idx_t nrow = cur_diagonal + 1; nrow >= 1; --nrow) {
            idx_t k = cur_diagonal - (nrow - 1);   /* column index for this element */
            seq_t val;
            if (k == 0) {
                val = (seq_t)nrow;                 /* T(n,0) = n */
            } else {
                seq_t prev = row_vals[(size_t)nrow];      /* T(n,k-1) */
                /* Collatz step: odd → 3*prev+1, even → prev/2 */
                if (prev & (seq_t)1) {
                    val = (prev << 1) + prev + (seq_t)1;   /* 3*prev + 1 */
                } else {
                    val = prev >> 1;                       /* prev / 2 */
                }
            }
            row_vals[(size_t)nrow] = val;    /* store updated row value for future steps */

            /* append val to the global sequence cache */
            if (seq_cache_size == seq_cache_cap) {
                size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
                seq_cache_cap = new_cap;
            }
            seq_cache[seq_cache_size++] = val;
        }

        ++cur_diagonal;   /* next diagonal will be generated on demand */
    }

    return seq_cache[(size_t)n - 1];   /* zero‑based indexing into the cache */
}
