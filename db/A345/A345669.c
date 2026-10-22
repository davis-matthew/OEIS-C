/*
 * A345669  Antidiagonal sums of array containing i-bonacci sequences
 *          nac(i,n), where nac(i,n) is the n-th i-bonacci number with
 *          nac(i,1..i) = 1 (see comments).
 * https://oeis.org/A345669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} nac(i,n-i+1) where nac(i,m) is the i‑bonacci
 *            number with first i terms = 1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation below runs in O(n²) time with a trivial
   constant factor.  For n <= 10000 it completes well under the
   required time limit.  All arithmetic is performed in seq_t, so
   it works for the arbitrary‑precision build as well. */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* the sequence is offset 1 */

    /* Allocate a buffer used as a circular sliding window.
       It can be reused in later calls. */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;
    if (buf_len < n) {
        seq_t *newbuf = (seq_t *)realloc(buf, (size_t)n * sizeof(seq_t));
        buf = newbuf;
        buf_len = n;
    }

    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t m = n - i + 1;          /* the index in the i‑bonacci sequence */

        seq_t term;
        if (i == 1) {                 /* the 1‑bonacci (all ones) */
            term = (seq_t)1;
        } else if (m <= i) {          /* terms 1..i are all 1 */
            term = (seq_t)1;
        } else {
            /* Initialise the sliding window with i copies of 1. */
            for (idx_t j = 0; j < i; ++j)
                buf[j] = (seq_t)1;

            seq_t sum = (seq_t)i;     /* sum of the current window */
            seq_t old = 0;            /* placeholder for old element */
            idx_t pos = 0;            /* write position in the window */

            /* Generate terms i+1 .. m.  The last generated term is
               the desired one. */
            for (idx_t k = i + 1; k <= m; ++k) {
                term = sum;                          /* new term           */
                old = buf[pos];                      /* element leaving    */
                buf[pos] = term;                     /* insert new element */
                sum += term - old;                   /* update window sum  */
                pos = (pos + 1) % i;                 /* advance circularly */
            }
        }

        result += term;   /* accumulating the antidiagonal sum */
    }

    return result;
}
