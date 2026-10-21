/*
 * A295729  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = 0, a(2) = 1, a(3) = 1.
 * https://oeis.org/A295729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) a(0) = -1, a(1) =
 *            0, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t) 0;
    if (n == 2) return (seq_t) 1;
    if (n == 3) return (seq_t) 1;

    seq_t a_minus4 = (seq_t)-1;   /* a(n-4)      */
    seq_t a_minus3 = (seq_t) 0;   /* a(n-3)      */
    seq_t a_minus2 = (seq_t) 1;   /* a(n-2)      */
    seq_t a_minus1 = (seq_t) 1;   /* a(n-1)      */
    seq_t current  = (seq_t) 0;   /* a(n)        */

    for (idx_t k = 4; k <= n; ++k)
    {
        current = a_minus1 + (seq_t)3 * a_minus2
                  - (seq_t)2 * a_minus3 - (seq_t)2 * a_minus4;

        /* Shift the window for next iteration */
        a_minus4 = a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = current;
    }

    return current;
}
