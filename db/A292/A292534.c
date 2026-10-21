/*
 * A292534  p-INVERT of the squares (A000290), where p(S) = 1 + S - S^2.
 * https://oeis.org/A292534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 5*a(n-1) - 12*a(n-2) + 22*a(n-3) - 16*a(n-4) + 7*a(n-5) -
 *            a(n-6) for n ≥ 7, with a(0)=‑1, a(1)=‑2, a(2)=4, a(3)=21,
 *            a(4)=30, a(5)=11, a(6)=80
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 21, i64 n <= 41, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the nth term of the sequence A292534 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t) 4;
    if (n == 3) return (seq_t)21;
    if (n == 4) return (seq_t)30;
    if (n == 5) return (seq_t)11;
    if (n == 6) return (seq_t)80;

    /* Pre‑compute the first seven values (indices 0..6) */
    seq_t a0 = (seq_t)-1;   /* a(0)   */
    seq_t a1 = (seq_t)-2;   /* a(1)   */
    seq_t a2 = (seq_t) 4;   /* a(2)   */
    seq_t a3 = (seq_t)21;   /* a(3)   */
    seq_t a4 = (seq_t)30;   /* a(4)   */
    seq_t a5 = (seq_t)11;   /* a(5)   */
    seq_t a6 = (seq_t)80;   /* a(6)   */

    /* Iteratively apply the recurrence until n is reached */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (seq_t)5  * a6
                    - (seq_t)12 * a5
                    + (seq_t)22 * a4
                    - (seq_t)16 * a3
                    + (seq_t)7  * a2
                    - a1;

        /* Shift window: drop oldest, push new */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;   /* a(n) */
}
