/*
 * A278438  Numbers m such that T(m) + 2*T(m+1) is a square, where T =
 *          A000217.
 * https://oeis.org/A278438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278438 – Numbers m such that T(m)+2*T(m+1) is a square
 * Recurrence: a(1)=7, a(2)=799, a(3)=78407
 * a(n)=99*a(n-1)-99*a(n-2)+a(n-3)   for n>3
 * (T is the triangular numbers, T(m)=m*(m+1)/2)
 * The function is recomputed from scratch each time; no external
 * memory allocation is required.
 */

#include <stdlib.h>  /* for abort() */

/* seq_t and idx_t types are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Handle indices before the offset – return 0 for safety. */
    if (n == 0) return 0;

    /* Initial values of the sequence. */
    seq_t a1 = (seq_t)7;     /* a(1) */
    seq_t a2 = (seq_t)799;   /* a(2) */
    seq_t a3 = (seq_t)78407; /* a(3) */

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Compute iteratively up to a(n). */
    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t next = (seq_t)99 * a3 - (seq_t)99 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
