/*
 * A288603  a(n) = 2*a(n-1) - a(n-3) for n >= 3, where a(0) = 2, a(1) = 4,
 *          a(2) = 6, a(3) = 8.
 * https://oeis.org/A288603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n >= 3, with * a(0) = 2, a(1) = 4,
 *            a(2) = 6, a(3) = 8
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first four base cases directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)8;

    /* For n >= 4, iterate the recurrence.  Use a sliding window
     * tracking the last three values a(n-3), a(n-2), a(n-1).  Only
     * arithmetic on seq_t is performed, so the function works for
     * all supported integer types. */
    seq_t a_nm3 = (seq_t)4; /* a(n-3) initially a(1) */
    seq_t a_nm2 = (seq_t)6; /* a(n-2) initially a(2) */
    seq_t a_nm1 = (seq_t)8; /* a(n-1) initially a(3) */
    seq_t a_n   = 0;        /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n   = (seq_t)2 * a_nm1 - a_nm3;   /* recurrence */
        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
