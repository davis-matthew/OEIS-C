/*
 * A276706  Indices of records in A249860.
 * https://oeis.org/A276706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=5, a(5)=6, a(6)=7 a(n) =
 *            (6*n-27-(-1)^n)/2 for n >= 7 where (-1)^n = 1 if n even, -1 if n
 *            odd. Implementation uses only integer arithmetic
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 1 .. 6 */
    if (n <= 6) {
        static const seq_t pre[6] = {1, 2, 4, 5, 6, 7};
        return pre[n - 1];
    }

    /* Compute (6*n - 27 - (-1)^n) / 2 for n >= 7 */
    seq_t val = ((seq_t)6 * (seq_t)n); /* 6*n */
    val -= 27;                          /* subtract 27 */
    /* Adjust by (-1)^n : if n even subtract 1, else add 1 */
    if ((n & 1) == 0) {                /* n is even */
        val -= 1;
    } else {                            /* n is odd */
        val += 1;
    }
    return val / 2;
}
