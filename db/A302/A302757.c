/*
 * A302757  a(n) is the smallest number whose greedy representation as a sum
 *          of terms of A126684 uses n terms.
 * https://oeis.org/A302757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*n - 5, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t a_prev = (seq_t)1;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 4*a(i-1) + 2*i - 5 */
        a_prev = 4 * a_prev + (seq_t)2 * i - (seq_t)5;
    }
    return a_prev;
}
