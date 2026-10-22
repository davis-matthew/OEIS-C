/*
 * A383976  In the binary expansion of n, expand bits 1 -> 11 and 0 -> 10.
 * https://oeis.org/A383976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=0..floor(log2(n))} 4^k * (2 + b_k) where b_k is
 *            the k-th bit of n (LSB is b_0)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 2;               /* a(0) = 2 */
    seq_t res = 0;
    idx_t m = n;
    int k = 0;
    while (m > 0) {
        /* (2 + bit) * 4^k   -->   (2 + bit) << (2*k) */
        seq_t term = ((seq_t)(2 + (m & 1))) << (2 * k);
        res += term;
        m >>= 1;
        ++k;
    }
    return res;
}
