/*
 * A285934  Number of connected induced (non-null) subgraphs of the perfect
 *          binary tree of height n.
 * https://oeis.org/A285934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1 * a(n)=b(n)^2 + 2*a(n-1) * where b(0)=1 * b(n)=1 +
 *            b(n-1)^2 (n≥1) * (so b(n)=A003095(n+1)) * * The routine
 *            recomputes the values from 0 up to n each call. * It uses only
 *            seq_t arithmetic and consumes O(1) extra memory
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t b      = 1;   /* current value of b(i) */
    seq_t result = 1;   /* current value of a(i) */

    if (n == 0)
        return result;

    for (idx_t i = 1; i <= n; ++i) {
        b      = 1 + b * b;    /* b(i) = 1 + b(i-1)^2 */
        seq_t sq = b * b;      /* b(i)^2 */
        result = sq + (result << 1);   /* a(i) = sq + 2*a(i-1) */
    }
    return result;
}
