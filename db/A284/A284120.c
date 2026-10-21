/*
 * A284120  Permutation of the positive integers: a(n) = A258746(A117120(n)) =
 *          A117120(A258746(n)).
 * https://oeis.org/A284120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k + ((n - 2^k) XOR ⌊2^(k‑1) / 3⌋) where k = ⌊log₂(n)⌋.
 *            This is the OEIS sequence A284120
 * Range:     u8 n <= 255, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against 0 */
        return (seq_t)0;

    /* compute k = floor(log2(n)) */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++k;

    /* m = 2^k */
    seq_t m = ((seq_t)1 << k);

    /* mask = floor( 2^(k-1) / 3 ) , with special handling for k==0 */
    seq_t mask = (k > 0) ? (((seq_t)1 << (k - 1)) / (seq_t)3) : (seq_t)0;

    /* a(n) = m + ( (n - m) XOR mask ) */
    seq_t r = (seq_t)n - m;
    return m + (r ^ mask);
}
