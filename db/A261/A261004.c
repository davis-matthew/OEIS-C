/*
 * A261004  Expansion of (-3-164*x-x^2)/(1-99*x+99*x^2-x^3).
 * https://oeis.org/A261004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -3,
 *              a(1) = -461,
 *              a(2) = -45343,
 *              a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3)  (n ≥ 3) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first three values explicitly. */
    if (n == 0) return (seq_t)-3;
    if (n == 1) return (seq_t)-461;
    if (n == 2) return (seq_t)-45343;

    /* Run the recurrence iteratively up to n. */
    seq_t a0 = (seq_t)-3;         /* a(0)   */
    seq_t a1 = (seq_t)-461;       /* a(1)   */
    seq_t a2 = (seq_t)-45343;     /* a(2)   */
    seq_t ai;                     /* temporary storage for a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)99 * a2 - (seq_t)99 * a1 + a0;
        a0 = a1;   /* shift previous values for next iteration */
        a1 = a2;
        a2 = ai;
    }
    return a2;  /* a(n) is now stored in a2 */
}
