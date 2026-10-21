/*
 * A290974  Alternating sum of row 2n of A022166.
 * https://oeis.org/A290974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..2n} (-1)^k * A022166(2n,k) Recurrence: a(0) =
 *            1, a(n) = (1 - 2^(2n-1)) * a(n-1) for n >= 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;      /* a(0) */
    seq_t pow   = (seq_t)2;      /* 2^(2*1-1) for the first step */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = (seq_t)1 - pow;      /* 1 - 2^(2i-1) */
        a_val = a_val * factor;             /* a(i) = a(i-1) * factor */
        pow = pow * (seq_t)4;               /* advance power: 2^(2(i+1)-1) = 4 * 2^(2i-1) */
    }
    return a_val;
}
