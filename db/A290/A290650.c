/*
 * A290650  a(1) = 1. For n > 1, a(n) = a(n-1)/2 if a(n-1) is even, a(n) =
 *          a(n-1)*n otherwise.
 * https://oeis.org/A290650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) / 2 if a(n-1) is even, a(n) =
 *            a(n-1) * n if a(n-1) is odd (OEIS offset 1). The function
 *            returns a(n) for any n >= 1 without any external memory
 *            allocation or side effects
 * Range:     u8 n <= 14, u16 n <= 21, u32 n <= 39, u64 n <= 62, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n = 0; return 0 to avoid UB */
        return 0;

    seq_t val = 1;        /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        if ((val & 1) == 0)     /* val is even */
            val >>= 1;          /* divide by 2 */
        else                    /* val is odd */
            val *= (seq_t)i;    /* multiply by i */
    }

    return val;
}
