/*
 * A270643  The sequence a of 1's and 2's starting with (1,2,2,1) such that
 *          a(n) is the length of the (n+3)rd run of a.
 * https://oeis.org/A270643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the 1/2 sequence whose run lengths are given by
   earlier terms, starting with (1,2,2,1).  The function returns the
   n-th term (1 ≤ n).  The sequence satisfies the OEIS specification
   for A270643. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;
    static size_t cap      = 0;   /* allocated size */
    static size_t sz       = 0;   /* current number of terms */
    static idx_t  next_run = 4;   /* next run number to generate (1‑based) */
    static seq_t  prev_val = (seq_t)1; /* value of the last finished run */

    /* initialise once */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!seq) abort();
        seq[0] = (seq_t)1;   /* a(1) */
        seq[1] = (seq_t)2;   /* a(2) */
        seq[2] = (seq_t)2;   /* a(3) */
        seq[3] = (seq_t)1;   /* a(4) */
        sz   = 4;
    }

    if (n == 0) return (seq_t)0;

    /* grow the sequence until it contains the n-th term */
    while (sz < n) {
        /* length of the run to generate: a[next_run-3] is seq[next_run-4] */
        seq_t len   = seq[next_run - 4];
        seq_t value = (prev_val == (seq_t)1) ? (seq_t)2 : (seq_t)1;

        for (idx_t i = 0; i < len; ++i) {
            if (sz == cap) {
                cap *= 2;
                seq = (seq_t*)realloc(seq, sizeof(seq_t) * cap);
                if (!seq) abort();
            }
            seq[sz++] = value;
        }
        prev_val = value;
        ++next_run;
    }

    return seq[n - 1];
}
