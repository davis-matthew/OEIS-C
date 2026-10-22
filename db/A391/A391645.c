/*
 * A391645  Number of integer compositions of n that are not the first sums of
 *          any finite nonnegative sequence.
 * https://oeis.org/A391645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows linear recurrence a(n) = 3*a(n-1) + 2*a(n-2) -
 *            11*a(n-3) + 3*a(n-4) + 6*a(n-5) with initial values a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=1. This recurrence is derived from
 *            the generating function G(x) =
 *            x^5/(1-3*x-2*x^2+11*x^3-3*x^4-6*x^5)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cached sequence values */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;      /* number of terms currently cached (sequence[0] = a(1)) */

    /* Initialize base values up to n=5 on first call */
    if (seq_len == 0) {
        seq_len = 5;
        seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
        if (!seq) return 0;   /* allocation failed – return 0 (unlikely in contest) */
        seq[0] = 0;  /* a(1) */
        seq[1] = 0;  /* a(2) */
        seq[2] = 0;  /* a(3) */
        seq[3] = 0;  /* a(4) */
        seq[4] = 1;  /* a(5) */
    }

    /* If the requested n is already cached, return it */
    if (n <= seq_len)
        return seq[n-1];

    /* Grow cache to size n */
    seq = (seq_t *)realloc(seq, n * sizeof(seq_t));
    if (!seq) return 0;   /* allocation failed – return 0 (unlikely) */

    /* Compute new terms using the recurrence */
    for (idx_t i = seq_len; i < n; ++i) {
        /* Terms correspond to a(i), a(i-1), ..., a(i-4) in 1‑based indexing */
        seq_t t = seq[i-1] * (seq_t)3; /* 3*a(i) */
        t += seq[i-2] * (seq_t)2;     /* +2*a(i-1) */
        t += seq[i-4] * (seq_t)3;     /* +3*a(i-3) */
        t += seq[i-5] * (seq_t)6;     /* +6*a(i-4) */
        seq_t sub = seq[i-3] * (seq_t)11;   /* 11*a(i-2) to be subtracted */
        t -= sub;                     /* subtract 11*a(i-2) */
        seq[i] = t;
    }
    seq_len = n;

    return seq[n-1];
}
