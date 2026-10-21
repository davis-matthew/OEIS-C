/*
 * A245829  Szeged index of the prism graph C_n X P_2 (n >=3).
 * https://oeis.org/A245829
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 if n is even; * a(n) = 3*n^3 - 4*n^2 + 2*n if n is
 *            odd * This implements the Szeged index of the prism graph C_n ×
 *            P_2 (offset 3)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  For n < 3 we return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Cast the index to seq_t once to preserve the appropriate type. */
    seq_t nn   = (seq_t)n;     /* n  */
    seq_t n2   = nn * nn;      /* n^2 */
    seq_t n3   = n2 * nn;      /* n^3 */

    seq_t result;
    if ((n & 1U) == 0)          /* even n */
    {
        result = n3 * (seq_t)3;
    }
    else                        /* odd n */
    {
        seq_t term2 = n2 * (seq_t)4;   /* 4*n^2 */
        seq_t term3 = nn * (seq_t)2;   /* 2*n   */
        result = n3 * (seq_t)3 - term2 + term3;
    }
    return result;
}
