/*
 * A302114  a(n) = 8*(n-1)*(2*n-3)*a(n-1) + ((-1)^n)*(n-1)*Product_{k=0..n-3}
 *          (2*k+1)^2 with a(0) = 0.
 * https://oeis.org/A302114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302114
 *  a(0) = 0, a(1) = 0, a(2) = 1
 *  For n ≥ 3:    (n-2) * a(n)
 *  = (n-1) * (12*n^2 - 36*n + 23) * a(n-1)
 *  + 8 * (n-2) * (n-1) * (2*n-5)^3 * a(n-2)
 *  The recurrence is memoised; the array grows up to the largest
 *  requested index so that calls may be in any order. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *big_seq = NULL;   /* memoised values a(0..max) */
static idx_t  seq_len = 0;      /* stored count (0 => none) */

/* ensure that a[0..n] are computed */
static void ensure_size(idx_t n)
{
    if (seq_len > n + 1)          /* already present */
        return;

    size_t new_size = (size_t)(n + 1);
    big_seq = (seq_t *)realloc(big_seq, new_size * sizeof(seq_t));
    if (!big_seq) abort();        /* allocation failure */

    /* compute values from seq_len up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            big_seq[i] = (seq_t)0;
        } else if (i == 2) {
            big_seq[i] = (seq_t)1;
        } else { /* i ≥ 3 */
            seq_t n_seq = (seq_t)i;
            seq_t a1    = big_seq[i - 1];
            seq_t a2    = big_seq[i - 2];

            /* term1 = (n-1)*(12*n^2 - 36*n + 23)*a(n-1) */
            seq_t poly  = (seq_t)12 * n_seq * n_seq
                        - (seq_t)36 * n_seq
                        + (seq_t)23;
            seq_t term1 = (n_seq - (seq_t)1) * poly * a1;

            /* term2 = 8*(n-2)*(n-1)*(2*n-5)^3*a(n-2) */
            seq_t diff  = (seq_t)2 * n_seq - (seq_t)5;
            seq_t pow3  = diff * diff * diff;          /* (2*n-5)^3 */
            seq_t factor = (seq_t)8
                         * (n_seq - (seq_t)2)
                         * (n_seq - (seq_t)1);
            seq_t term2 = factor * pow3 * a2;

            seq_t rhs = term1 + term2;
            big_seq[i] = rhs / (n_seq - (seq_t)2);     /* exact division */
        }
    }
    seq_len = new_size;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return big_seq[(size_t)n];
}
