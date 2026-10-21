/*
 * A255973  Trace of the minimal alternating Fibonacci representation of n.
 * https://oeis.org/A255973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 107, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive formula for the trace of the minimal alternating Fibonacci
   representation (A255973):
   
   a(0) = 0.
   For n > 0, let F be the smallest Fibonacci number ≥ n.
   If F == n, then a(n) = F.
   Otherwise a(n) = -a(F - n).   */

/* Necessary includes for the build.  The types seq_t and idx_t are supplied
   by the build system; do not define them here. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0ULL)            /* 0 is the offset of the sequence */
        return 0;

    /* Find the smallest Fibonacci number F >= n. */
    seq_t f1 = (seq_t)1;
    seq_t f2 = (seq_t)1;
    seq_t nn = (seq_t)n;

    /* Special case: n == 1 is handled by the loop below, but keeping a
       small optimisation avoids an extra iteration when n == 1. */
    if (nn == 1) 
        return 1;

    while (f2 < nn) {
        seq_t tmp = f1 + f2;   /* Fibonacci recurrence */
        f1 = f2;
        f2 = tmp;
    }
    seq_t F = f2;              /* Smallest Fibonacci >= n */

    /* Difference k = F - n is always < n and fits in idx_t. */
    seq_t k = F - nn;
    if (k == 0)                         /* n itself is Fibonacci */
        return F;
    return -a((idx_t)k);                /* recursion with sign flip */
}
