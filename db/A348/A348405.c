/*
 * A348405  a(0) = 1, a(n) + a(n+1) = round(2^n/9), n >= 0.
 * https://oeis.org/A348405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 35, i64 n <= 67, i128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348405:
 *   a(0) = 1,
 *   a(n) + a(n+1) = round(2^n / 9)   (n ≥ 0),
 *   where round(x) is the nearest integer (ties rounded up). */

seq_t a(idx_t n)
{
    seq_t a_cur = (seq_t)1;   /* a(0) */
    if (n == 0)
        return a_cur;

    seq_t b_cur = (seq_t)0;   /* round(2^0 / 9) = 0 */
    unsigned r = 1U;          /* 2^0 mod 9 */

    for (idx_t i = 0; i < n; ++i)
    {
        unsigned d     = (r + 4U) / 9U;          /* floor((r+4)/9)   */
        unsigned delta = (2U * r + 4U) / 9U;      /* floor((2r+4)/9)  */

        seq_t a_next = b_cur - a_cur;

        /* b_{i+1} = 2*b_i - 2*d_i + delta_i */
        seq_t b_next = b_cur * (seq_t)2
                       - (seq_t)2 * (seq_t)d
                       + (seq_t)delta;

        a_cur = a_next;
        b_cur = b_next;

        r = (r * 2U) % 9U;      /* next 2^i mod 9 */
    }

    return a_cur;
}
