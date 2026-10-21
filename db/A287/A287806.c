/*
 * A287806  Number of senary sequences of length n such that no two
 *          consecutive terms have distance 1.
 * https://oeis.org/A287806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 3*a(n-3) for n > 3 with initial
 *            values a(0) = 1, a(1) = 6, a(2) = 26, a(3) = 114 (OEIS A287806)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)26;
    if (n == 3) return (seq_t)114;

    /* Iterate the recurrence forward. */
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)6;   /* a(1)   */
    seq_t a2 = (seq_t)26;  /* a(2)   */
    seq_t a3 = (seq_t)114; /* a(3)   */

    /* Compute up to the requested n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = (seq_t)5 * a3 - (seq_t)2 * a2 - (seq_t)3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
