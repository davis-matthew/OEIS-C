/*
 * A362270  a(1) = 1, then subtract, multiply, and add 2, 3, 4; 5, 6, 7; ...
 *          in that order.
 * https://oeis.org/A362270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; For n > 1: if n mod 3 = 1, a(n) = a(n‑1) + n; if n mod
 *            3 = 2, a(n) = a(n‑1) - n; if n mod 3 = 0, a(n) = a(n‑1) * n
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 26, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case directly. */
    if (n == 1)
        return (seq_t)1;

    seq_t val = (seq_t)1;   /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t step = (seq_t)i;   /* convert index to seq_t */
        idx_t mod = i % 3;
        if (mod == 1)
            val += step;         /* + n */
        else if (mod == 2)
            val -= step;         /* - n */
        else /* mod == 0 */
            val *= step;         /* * n */
    }
    return val;
}
