/*
 * A331533  Irregular table read by rows; for n >= 0, the n-th row corresponds
 *          to the nonnegative integers k such that (n^2) AND (k^2) = k^2, in
 *          ascending order (where AND denotes the bitwise AND operator).
 * https://oeis.org/A331533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 4766, u32 n <= 7118, u64 n <= 7118,
 *            u128 n <= 7118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A331533: irregular table read by rows.  The routine builds
   rows incrementally, caching the flattened sequence as needed and
   returning the n‑th element.  No external libraries are required. */

#include <stdlib.h>

static seq_t *seq_buf  = NULL;   /* cached sequence terms */
static size_t seq_len  = 0;      /* number of stored terms   */
static size_t seq_cap  = 0;      /* allocated capacity       */
static idx_t next_row  = 0;      /* next row index to process*/

/* Ensure the buffer can hold at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the n-th term of A331533. */
seq_t a(idx_t n)
{
    /* Expand cached sequence until it contains at least n+1 terms. */
    while (seq_len <= (size_t)n) {
        seq_t n_val = next_row;                 /* convert row index to seq_t */
        seq_t n_sq  = n_val * n_val;             /* n^2 */

        /* Enumerate all k from 0..next_row and test the condition. */
        for (idx_t k = 0; k <= next_row; ++k) {
            seq_t k_val = k;
            seq_t k_sq  = k_val * k_val;
            if ((n_sq & k_sq) == k_sq) {
                /* Append k to the cache. */
                ensure_capacity(seq_len + 1);
                seq_buf[seq_len++] = k_val;
            }
        }
        ++next_row;
    }
    return seq_buf[(size_t)n];
}
