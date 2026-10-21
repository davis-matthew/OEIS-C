/*
 * A290917  p-INVERT of the positive integers, where p(S) = (1 - S)^2.
 * https://oeis.org/A290917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) with initial
 *            values a(0)=2, a(1)=7, a(2)=22, a(3)=67
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return 2;
    if (n == 1) return 7;
    if (n == 2) return 22;
    if (n == 3) return 67;

    /* Recurrence computation */
    seq_t a0 = 2;   /* a(n-4)   */
    seq_t a1 = 7;   /* a(n-3)   */
    seq_t a2 = 22;  /* a(n-2)   */
    seq_t a3 = 67;  /* a(n-1)   */

    seq_t ai;       /* a(n)     */
    idx_t i = 4;
    for ( ; i <= n; ++i) {
        ai = 6*a3 - 11*a2 + 6*a1 - a0;
        a0 = a1;   /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
