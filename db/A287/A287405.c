/*
 * A287405  a(n) = (7*(cot(1*Pi/7))^2)^n + (7*(cot(2*Pi/7))^2)^n +
 *          (7*(cot(4*Pi/7))^2)^n.
 * https://oeis.org/A287405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 147*a(n-2) + 49*a(n-3) a(0) = 3, a(1) = 35,
 *            a(2) = 931
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the n-th term of A287405.  The sequence is defined by the
   linear recurrence a(n)=35*a(n-1)-147*a(n-2)+49*a(n-3).  The
   calculation is performed using only integer operations that are
   defined for the generic seq_t type.  No dynamic memory allocation is
   needed, and the function works correctly for any build that provides
   seq_t and idx_t as specified. */
seq_t a(idx_t n)
{
    /* Handle the initial three values explicitly. */
    if (n == (idx_t)0) {
        return (seq_t)3;
    }
    if (n == (idx_t)1) {
        return (seq_t)35;
    }
    if (n == (idx_t)2) {
        return (seq_t)931;
    }

    /* Use three rolling variables to track the last three terms. */
    seq_t prev3 = (seq_t)3;   /* a(n-3) */
    seq_t prev2 = (seq_t)35;  /* a(n-2) */
    seq_t prev1 = (seq_t)931; /* a(n-1) */
    seq_t next;

    /* Iterate from 3 up to n, computing each term from the recurrence. */
    for (idx_t i = (idx_t)3; i <= n; ++i) {
        /* Compute next = 35*prev1 - 147*prev2 + 49*prev3.
           To avoid unsigned underflow in the subtraction, perform
           the addition first.  The intermediate results are guaranteed
           to fit in seq_t when the final term does, as required by the
           specification. */
        seq_t term1 = (seq_t)35 * prev1;
        seq_t term2 = (seq_t)49 * prev3;
        seq_t term_sub = (seq_t)147 * prev2;

        next = term1 + term2;
        next -= term_sub;

        /* Shift the window for the next iteration. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return next;
}
