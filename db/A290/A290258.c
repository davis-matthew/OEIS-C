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
 * Validated: partial, pass, 54 terms through n = 55 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Row n = merge(2 · row(n‑1), 4 · row(n‑2)+3)  
   with row(2) = {3}  ,  row(3) = {6}.  The sequence a(n) lists all rows
   in order, so a(2)=3, a(3)=6, a(4)=12, … */
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper that builds the next row from the previous two rows.  */
static seq_t *next_row(const seq_t *prev1, size_t sz1,
                       const seq_t *prev2, size_t sz2,
                       size_t *out_sz)
{
    size_t total = sz1 + sz2;
    seq_t *row = mem_new(seq_t, total);
    size_t i = 0, j = 0, k = 0;
    while (i < sz1 && j < sz2) {
        seq_t a = prev1[i] << 1;                 /* 2·x   */
        seq_t b = (prev2[j] << 2) + 3;           /* 4·y+3 */
        if (a <= b) {
            row[k++] = a;
            ++i;
        } else {
            row[k++] = b;
            ++j;
        }
    }
    while (i < sz1) {
        row[k++] = prev1[i++] << 1;
    }
    while (j < sz2) {
        row[k++] = (prev2[j++] << 2) + 3;
    }
    *out_sz = total;
    return row;
}

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
        row_n2 = mem_new(seq_t, 1);
        row_n2[0] = 3;
        sz_n2 = 1;

        /* row 3 : {6} */
        row_n1 = mem_new(seq_t, 1);
        row_n1[0] = 6;
        sz_n1 = 1;

        seq_len = 2;
        seq = mem_new(seq_t, seq_len);
        seq[0] = 3;
        seq[1] = 6;
    }

    /* Generate rows until we have enough terms. */
    while (seq_len < (size_t)(n - 1)) {
        size_t new_sz = 0;
        seq_t *new_row = next_row(row_n1, sz_n1, row_n2, sz_n2, &new_sz);

        /* Append the new row to the global sequence. */
        seq_t *new_seq = mem_new(seq_t, (seq_len + new_sz));
        mem_copy(new_seq, seq, seq_len * sizeof(seq_t));
        mem_copy(new_seq + seq_len, new_row, new_sz * sizeof(seq_t));
        mem_free(seq);
        seq = new_seq;
        seq_len += new_sz;

        /* Advance the row pointers. */
        mem_free(row_n2);
        row_n2 = row_n1;
        sz_n2  = sz_n1;
        row_n1 = new_row;
        sz_n1  = new_sz;
    }

    return seq[n - 2];
}
