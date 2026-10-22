/*
 * A385972  The long legs of the triangles defined in A365577.
 * https://oeis.org/A385972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * Π_{k=0}^{n-2} 2^{2^k}·(2^{2^k}+1) (a(1)=4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)4;          /* a(1) = 4 */

    seq_t result = (seq_t)4;      /* current product */
    seq_t power  = (seq_t)2;      /* 2^{2^0} = 2 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t term  = power * (power + (seq_t)1);  /* 2^{2^{i-1}}·(2^{2^{i-1}}+1) */
        result      = result * term;               /* multiply into product */
        power       = power * power;              /* advance to 2^{2^i} */
    }

    return result;
}
