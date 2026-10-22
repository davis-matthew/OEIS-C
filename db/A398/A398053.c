/*
 * A398053  Upper (1,2/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7), with
 *            (a(0), …, a(6)) = (0, 2, 5, 10, 17, 25, 36)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* base values a(0)..a(6) */
    static const seq_t base_vals[7] = {0, 2, 5, 10, 17, 25, 36};

    /* static storage for computed values */
    static seq_t *seq = NULL;
    static idx_t seq_cap   = 0;   /* allocated capacity of seq array   */
    static idx_t seq_len   = 0;   /* number of elements currently stored */

    /* Ensure we have enough capacity to store index n */
    if (n + 1 > seq_cap) {
        idx_t new_cap = seq_cap > 0 ? seq_cap : 16;    /* start with 16 entries */
        while (new_cap < n + 1) new_cap <<= 1;        /* double until enough */

        seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (seq) {
            memcpy(new_seq, seq, seq_len * sizeof(seq_t));
            free(seq);
        }
        seq = new_seq;
        seq_cap = new_cap;
    }

    /* Fill in the initial seven terms if they have not yet been stored */
    if (seq_len <= 6) {
        for (idx_t i = seq_len; i <= 6; ++i)
            seq[i] = base_vals[i];
        if (seq_len < 7) seq_len = 7;
    }

    /* Compute missing terms up to n using the recurrence */
    if (n >= 7 && seq_len <= n) {
        for (idx_t i = seq_len; i <= n; ++i) {
            seq[i] = 2 * seq[i-1] - seq[i-2] + seq[i-5]
                    - 2 * seq[i-6] + seq[i-7];
        }
        seq_len = n + 1;   /* we now have values up to index n */
    }

    /* seq_len may already be > n+1 if a larger index was computed before */
    return seq[n];
}
