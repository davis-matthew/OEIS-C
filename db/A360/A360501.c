/*
 * A360501  Number of edges added at n-th generation of hexagonal graph
 *          constructed in first quadrant (see Comments for precise
 *          definition).
 * https://oeis.org/A360501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n=0; a(1)=1; a(2)=1; a(n)=n for even n≥4; a(n)=n +
 *            floor((n-1)/4) - 1 for odd n≥3
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;
    if ((n & 1) == 0)            /* even n ≥ 4 */
        return (seq_t)n;
    /* odd n ≥ 3 */
    seq_t part = (seq_t)((n - 1) / 4);
    return (seq_t)n + part - (seq_t)1;
}
