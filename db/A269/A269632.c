/*
 * A269632  a(1)=1. At any step consider the terms as two streams of digits:
 *          the first read from left to right and the second from right to
 *          left. Sum the two numbers and add them as new term of the
 *          sequence. Repeat.
 * https://oeis.org/A269632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1
   For n > 1, let X be the decimal concatenation of a(1)..a(n–1)
   read left‑to‑right, and revX its reverse (right‑to‑left).
   Then a(n) = X + revX.
   Update X and revX by appending a(n).  */
#include "../utils/pow10.h"
#include "../utils/rev_decimal.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t X    = (seq_t)1;   /* concatenation of 1   */
    seq_t revX = (seq_t)1;   /* reverse of 1         */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t cur = X + revX;       /* a(i)                  */

        if (i == n)
            return cur;

        int d_X    = digit_len(X);      /* digits of old X       */
        int d_cur  = digit_len(cur);    /* digits of cur         */

        seq_t pow10_X   = pow10((idx_t)d_X);
        seq_t pow10_cur = pow10((idx_t)d_cur);
        seq_t rev_cur   = rev_decimal(cur);

        X    = X * pow10_cur + cur;
        revX = rev_cur * pow10_X + revX;
    }

    /* unreachable */
    return (seq_t)0;
}
