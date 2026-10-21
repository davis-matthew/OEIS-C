/*
 * A246641  Sequence a(n) = (1 + A007805(n))/2, appearing in a certain
 *          touching problem for three circles and a chord, together with
 *          A007805.
 * https://oeis.org/A246641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 19*a(n-2) + a(n-3); * Initial values: a(0)=1,
 *            a(1)=9, a(2)=153
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)153;

    /* Use a sliding window of the last three values. */
    seq_t a0 = (seq_t)1;    /* a(n-3) */
    seq_t a1 = (seq_t)9;    /* a(n-2) */
    seq_t a2 = (seq_t)153;  /* a(n-1) */
    seq_t an = a2;          /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)19 * a2 - (seq_t)19 * a1 + a0;
        a0 = a1;  /* shift the window */
        a1 = a2;
        a2 = an;
    }
    return an;
}
