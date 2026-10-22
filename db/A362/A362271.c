/*
 * A362271  a(1) = 1, then add, subtract and multiply 2, 3, 4; 5, 6, 7; ... in
 *          that order.
 * https://oeis.org/A362271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n>1, a(n) = a(n-1)+n if n mod 3 == 2, a(n) =
 *            a(n-1)-n if n mod 3 == 0, a(n) = a(n-1)*n if n mod 3 == 1
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 27, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1, and n=0 is ignored by offset */
        return 1;

    seq_t val = 1;         /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t r = i % 3;   /* i mod 3 */
        if (r == 2)        /* addition case */
            val += (seq_t)i;
        else if (r == 0)   /* subtraction case */
            val -= (seq_t)i;
        else               /* multiplication case (r == 1) */
            val *= (seq_t)i;
    }
    return val;
}
