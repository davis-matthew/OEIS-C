/*
 * A243800  Number of simple connected graphs on n nodes whose maximum size of
 *          an independent edge set is equal to 2.
 * https://oeis.org/A243800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n<4, a(4)=5, a(5)=20, a(n)=C(n,2)+1 for n>=6
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4ULL) return 0;
    if (n == 4ULL) return 5;
    if (n == 5ULL) return 20;
    /* for n >= 6: a(n) = n*(n-1)/2 + 1 */
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn - 1);
    res = res / 2U;
    res = res + 1;
    return res;
}
