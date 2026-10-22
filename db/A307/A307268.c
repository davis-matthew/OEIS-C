/*
 * A307268  Partial sums of the Lucas numbers of the form L(3n+2) (A163063).
 * https://oeis.org/A307268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (L(3*n+4)-1)/2 Recurrence: a(0)=3, a(1)=14, a(2)=61
 *            a(n)=5*a(n-1)-3*a(n-2)-a(n-3) for n>2 The function returns with
 *            exact arithmetic while the result fits in the compile‑time
 *            chosen type `seq_t`
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)61;

    /* Compute iteratively using the recurrence
       Only three previous values are kept */
    seq_t a0 = (seq_t)3;        /* a(0) */
    seq_t a1 = (seq_t)14;       /* a(1) */
    seq_t a2 = (seq_t)61;       /* a(2) */
    seq_t a3;                  /* will hold a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        a3 = (seq_t)(5) * a2 - (seq_t)(3) * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
