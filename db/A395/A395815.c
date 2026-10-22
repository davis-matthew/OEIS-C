/*
 * A395815  Number of perfect matchings in the complete tripartite graph
 *          K_{2n,2n,2n}.
 * https://oeis.org/A395815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n)!/n!)^3 = 8*(2n-1)^3 * a(n-1), a(0)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* t = 2k-1 */
        seq_t t = (seq_t)2 * k - 1;
        /* factor = 8 * t^3 */
        seq_t factor = (seq_t)8 * t * t * t;
        res = res * factor;
    }
    return res;
}
