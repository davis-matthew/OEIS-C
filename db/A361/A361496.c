/*
 * A361496  Inventory of positions as an irregular table; row 0 contains 0,
 *          subsequent rows contain the 0-based positions (mod 2) of 0's,
 *          followed by the positions (mod 2) of 1's in prior rows flattened.
 * https://oeis.org/A361496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A361496:  row construction – each new row consists of the
   0‑based positions (mod 2) of 0’s in all previous rows,
   followed by the positions (mod 2) of 1’s.  The table starts:
     0,
     0,
     0, 1,
     0, 1, 0, 1, ...  */

#include <stdlib.h>

static seq_t *seq_arr = NULL;   /* flattened sequence */
static idx_t seq_len  = 0;      /* number of stored terms */

/* Add the next row to the flattened sequence.  
   Assumes all existing terms are 0 or 1. */
static void grow_row(void)
{
    idx_t old_len = seq_len;
    idx_t new_len = old_len * 2;

    seq_arr = (seq_t *)realloc(seq_arr, sizeof(seq_t) * new_len);
    if (!seq_arr) abort(); /* memory allocation failed */

    idx_t write = old_len;   /* position to write new terms */

    /* first write positions of zeros (mod 2) */
    for (idx_t i = 0; i < old_len; ++i) {
        if (seq_arr[i] == 0) {
            seq_arr[write++] = (seq_t)(i & 1);
        }
    }
    /* then write positions of ones (mod 2) */
    for (idx_t i = 0; i < old_len; ++i) {
        if (seq_arr[i] == 1) {
            seq_arr[write++] = (seq_t)(i & 1);
        }
    }
    seq_len = new_len;
}

/* Sequence entry function.  Builds rows on demand so that any order
   of calls is supported. */
seq_t a(idx_t n)
{
    /* initialise with the first term if needed */
    if (seq_len == 0) {
        seq_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_arr) abort();
        seq_arr[0] = 0;
        seq_len  = 1;
    }

    /* generate rows until we have at least n+1 terms */
    while (seq_len <= n) {
        grow_row();
    }

    return seq_arr[n];
}
