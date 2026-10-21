/*
 * A263394  a(n) = Product_{i=1..n} (3^i - 2^i).
 * https://oeis.org/A263394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..n} (3^i - 2^i)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 1
       (the empty product), though this case is outside the OEIS
       offset and is not checked. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t pow3 = (seq_t)1;   /* will become 3^i */
    seq_t pow2 = (seq_t)1;   /* will become 2^i */

    for (idx_t i = 1; i <= n; ++i)
    {
        pow3 = pow3 * (seq_t)3;
        pow2 = pow2 * (seq_t)2;
        result = result * (pow3 - pow2);
    }
    return result;
}
