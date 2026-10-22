/*
 * A348080  A variant of Van Eck's sequence: For n >= 1, if there exists an m
 *          < n such that a(m) = a(n), take the largest such m and set a(n+1)
 *          = n XOR m; otherwise a(n+1) = 0. Start with a(1)=0.
 * https://oeis.org/A348080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0;
   For n ≥ 1 let m be the largest index < n with a(m) = a(n).
   Then a(n+1) = n XOR m   if such m exists, otherwise a(n+1) = 0. */

#include <stdlib.h>

static seq_t *seq = NULL;        /* array storing a(1), a(2), … */
static size_t  seq_cap = 0;      /* capacity of the array */
static size_t  seq_len = 0;      /* number of terms already computed */

seq_t a(idx_t n)
{
    /* compute up to index n (1‑based) if necessary */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;    /* a(1) = 0 */
        seq_len = 1;
    }

    while (seq_len < n) {
        /* current index n (1‑based) whose next term we want to compute */
        idx_t cur_n = seq_len;                /* n in the recurrence */
        seq_t cur_val = seq[seq_len - 1];     /* a(n) = a(cur_n) */

        /* find largest m < cur_n with a(m) = cur_val */
        idx_t m = 0;
        for (idx_t j = seq_len - 1; j > 0; --j) {
            if (seq[j - 1] == cur_val) {
                m = j;   /* m is 1‑based */
                break;
            }
        }

        seq_t next_val;
        if (m == 0) {
            next_val = 0;
        } else {
            next_val = ((seq_t)cur_n) ^ ((seq_t)m);
        }

        /* grow array if needed */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        seq[seq_len] = next_val;  /* store a(cur_n + 1) */
        seq_len++;
    }

    return seq[n - 1];
}
