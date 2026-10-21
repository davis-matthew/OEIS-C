/*
 * A290258  Triangle read by rows: row n (>=2) contains in increasing order
 *          the integers for which the binary representation has length n and
 *          all runs of 1's have even length.
 * https://oeis.org/A290258
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   next_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Row n = merge(2 · row(n‑1), 4 · row(n‑2)+3)  
   with row(2) = {3}  ,  row(3) = {6}.  The sequence a(n) lists all rows
   in order, so a(2)=3, a(3)=6, a(4)=12, … */
#include <stdlib.h>
#include <string.h>
#include "../utils/next_row.h"

/* Main function requested by the test harness. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* defined only for n >= 2 */

    /* Static state: the accumulated sequence and the last two rows. */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static seq_t *row_n2   = NULL;  /* row n-2   */
    static seq_t *row_n1   = NULL;  /* row n-1   */
    static size_t sz_n2    = 0;
    static size_t sz_n1    = 0;

    /* One‑time initialization of the first two rows and the first two
       terms of the global sequence. */
    if (seq == NULL) {
        /* row 2 : {3} */
        row_n2 = (seq_t *)malloc(sizeof(seq_t));
        row_n2[0] = 3;
        sz_n2 = 1;

        /* row 3 : {6} */
        row_n1 = (seq_t *)malloc(sizeof(seq_t));
        row_n1[0] = 6;
        sz_n1 = 1;

        seq_len = 2;
        seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
        seq[0] = 3;
        seq[1] = 6;
    }

    /* Generate rows until we have enough terms. */
    while (seq_len < (size_t)(n - 1)) {
        size_t new_sz = 0;
        seq_t *new_row = next_row(row_n1, sz_n1, row_n2, sz_n2, &new_sz);

        /* Append the new row to the global sequence. */
        seq_t *new_seq = (seq_t *)malloc((seq_len + new_sz) * sizeof(seq_t));
        memcpy(new_seq, seq, seq_len * sizeof(seq_t));
        memcpy(new_seq + seq_len, new_row, new_sz * sizeof(seq_t));
        free(seq);
        seq = new_seq;
        seq_len += new_sz;

        /* Advance the row pointers. */
        free(row_n2);
        row_n2 = row_n1;
        sz_n2  = sz_n1;
        row_n1 = new_row;
        sz_n1  = new_sz;
    }

    return seq[n - 2];
}
