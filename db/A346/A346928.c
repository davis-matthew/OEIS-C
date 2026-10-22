/*
 * A346928  Irregular triangle read by rows; the n-th row contains, in
 *          ascending order, the distinct integers of the form n! / m! (with 1
 *          <= m <= n) that do not appear in former rows.
 * https://oeis.org/A346928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A346928.  
   The sequence is generated row‑by‑row: for each n >= 1 the
   values n! / m! (1 <= m <= n) are formed, sorted in ascending
   order, and every value that has not appeared in any previous
   row is appended to the flat sequence.  The routine keeps a
   growing array of all values that have already appeared and
   searches it linearly when a new candidate is produced.  The
   function returns the n‑th term, with a(1) == 1.  The array
   grows only as far as needed for the requested index.  The
   code is valid C11 and C++17; it uses malloc/realloc but casts
   the returned void pointer explicitly to support C++. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Check whether val is already in the first len elements of arr */
static bool contains_seq(const seq_t *arr, size_t len, seq_t val)
{
    for (size_t i = 0; i < len; ++i) {
        if (arr[i] == val) return true;
    }
    return false;
}

/* Ensure that *p has capacity for at least new_cap seq_t elements.
   The returned pointer (possibly reallocated) is stored in *p. */
static void ensure_cap(seq_t **p, size_t *cap, size_t new_cap)
{
    if (*cap >= new_cap) return;
    size_t new_size = (*cap == 0) ? 8 : (*cap << 1);
    if (new_size < new_cap) new_size = new_cap;
    seq_t *tmp = (seq_t *)realloc(*p, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    *p = tmp;
    *cap = new_size;
}

/* Return the n-th term of sequence A346928.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static storage for all values generated so far. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_row = 1;   /* the next row to process */

    /* Special case: if the caller asks for a term that has already
       been generated in a previous call, we can return it immediately. */
    if (n <= seq_len) return seq[n - 1];

    /* Generate rows until we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* Start with value n! / n! = 1 for the current row. */
        seq_t val = 1;

        /* Check if 1 has appeared before; if not, add it. */
        if (!contains_seq(seq, seq_len, val)) {
            ensure_cap(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = val;
            if (seq_len == (size_t)n) return seq[n - 1];
        }

        /* Generate the remaining values for this row:
           multiply by i = current row number, then by i-1, ... until 2. */
        for (idx_t i = next_row; i > 1; --i) {
            val = val * (seq_t)i;   /* multiply by i (i = m+1) */
            if (!contains_seq(seq, seq_len, val)) {
                ensure_cap(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = val;
                if (seq_len == (size_t)n) return seq[n - 1];
            }
        }
        ++next_row;   /* proceed to next row */
    }

    /* Should never reach here, but satisfy the compiler. */
    return seq[n - 1];
}
