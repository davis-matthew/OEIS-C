/*
 * A274106  Triangle read by rows: T(n,k) = total number of configurations of
 *          k nonattacking bishops on the white squares of an n X n chessboard
 *          (0 <= k <= n-1+[n=0]).
 * https://oeis.org/A274106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle entries: T(n,k) = T(n-1,k) + (n-k+1-(n%2)) *
 *            T(n-1,k-1), with T(n,0)=1 for all n ≥ 0, and T(0,0)=1. The
 *            sequence a(n) is the triangle read by rows (row 0: k=0; row n:
 *            k=0..n-1)
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoize flattened triangle */
    static seq_t *flattened = NULL;
    static size_t  flat_cap = 0;
    static size_t  flat_len = 0;

    /* State of the previous row */
    static seq_t *prev_row  = NULL;
    static size_t  prev_len = 0;
    static idx_t  current_row = 0;   /* last computed row number */

    /* Initialise on first call */
    if (flat_cap == 0) {
        flat_cap   = 1024;
        flattened  = (seq_t *)malloc(flat_cap * sizeof(seq_t));
    }

    if (flat_len == 0) {            /* row 0: T(0,0)=1 */
        prev_row  = (seq_t *)malloc(sizeof(seq_t));
        prev_len  = 1;
        prev_row[0] = (seq_t)1;
        flattened[0] = prev_row[0];
        flat_len = 1;
        current_row = 0;
    }

    /* Generate rows until we have at least N+1 elements */
    while (flat_len <= (size_t)n) {
        idx_t   next_row   = current_row + 1;
        size_t  len_next   = (next_row == 0) ? 1 : (size_t)next_row;  /* length of row */
        seq_t *curr_row = (seq_t *)malloc(len_next * sizeof(seq_t));

        curr_row[0] = (seq_t)1;  /* T(n,0)=1 */

        for (size_t k = 1; k < len_next; ++k) {
            seq_t left  = (k < prev_len) ? prev_row[k] : (seq_t)0;
            seq_t right = prev_row[k - 1];
            seq_t coef  = (seq_t)(next_row - k + 1) - ( (seq_t)(next_row & 1) );
            curr_row[k] = left + coef * right;
        }

        /* Append to flattened array – grow if needed */
        if (flat_len + len_next > flat_cap) {
            while (flat_len + len_next > flat_cap) flat_cap <<= 1;
            flattened = (seq_t *)realloc(flattened, flat_cap * sizeof(seq_t));
        }
        for (size_t i = 0; i < len_next; ++i)
            flattened[flat_len++] = curr_row[i];

        /* Advance state */
        free(prev_row);
        prev_row  = curr_row;
        prev_len  = len_next;
        current_row = next_row;
    }

    return flattened[(size_t)n];
}
