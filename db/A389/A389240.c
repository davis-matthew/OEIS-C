/*
 * A389240  Start the sequence S with S(1) = n and extend S with S(m+1) =
 *          S(m)/2 when S(m) is even, otherwise with S(m) + the smallest odd
 *          number not yet added. a(n) is the number of steps to reach 1, or
 *          -1 if 1 is never reached.
 * https://oeis.org/A389240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = number of steps needed to reach 1 by the process defined
 *            in A389240. The process starts with S(1)=n. For each subsequent
 *            term if S(m) is even, then S(m+1) = S(m)/2, if S(m) is odd, then
 *            S(m+1) = S(m) + d, where d is the smallest odd integer not yet
 *            used as the additive increment. d starts...
 * Range:     i8 n <= 60, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>   /* for MAXINT, if needed */

#define MAX_STEPS 2000000ULL   /* upper bound to avoid infinite loops */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;          /* current term of the sequence */
    seq_t d   = 1;                  /* next unused odd increment   */
    idx_t  step = 0;                /* number of steps taken       */

    /* Special case: if starting value is already 1 */
    if (cur == (seq_t)1)
        return (seq_t)0;

    /* Simulate the process until either 1 is reached, the step
       counter exceeds MAX_STEPS, or an overflow would occur. */
    while (step < MAX_STEPS) {
        /* If cur is even, halve it. */
        if ((cur & (seq_t)1) == 0) {
            cur = cur >> 1;
        } else {
            /* cur is odd.  Add the next unused odd increment d. */
            /* For 64‑bit builds, check that the addition does not
               overflow before performing it. */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
            /* C11 provides builtin overflow check */
            unsigned __int128 sum = (unsigned __int128)cur + d;
            if (sum > (unsigned __int128)LLONG_MAX)
                return (seq_t)-1;
            cur = (seq_t)sum;
#else
            /* Fallback: assume no overflow for the tested range. */
            cur = cur + d;
#endif
            d += 2;            /* advance to the next unused odd */
        }

        step++;

        if (cur == (seq_t)1)
            return (seq_t)step;
    }

    /* If we left the loop without reaching 1, assume the
       sequence never reaches 1 and return –1. */
    return (seq_t)-1;
}
