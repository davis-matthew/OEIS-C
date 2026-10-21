/*
 * A283483  Sums of distinct nonzero terms of A003462: a(n) = Sum_{k>=0}
 *          A030308(n,k)*A003462(1+k).
 * https://oeis.org/A283483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i: bit i of n is set} (3^(i+1)-1)/2 * This is
 *            derived from the identities: * a(2^k) = A003462(k+1) =
 *            (3^(k+1)-1)/2 * a(n) = a(n-2^floor(log2(n))) +
 *            A003462(floor(log2(n))+1) * which leads to a(n) being the
 *            weighted sum of set bits in the binary * expansion of n
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum   = 0;
    seq_t pow3  = 1;               /* 3^0 */
    for (idx_t i = 0; (n >> i) != 0; ++i) {
        if ((n >> i) & 1) {
            /* term = (3^(i+1)-1)/2 = ((pow3*3)-1)/2 */
            seq_t term = pow3 * (seq_t)3 - (seq_t)1;
            term /= (seq_t)2;
            sum += term;
        }
        pow3 *= (seq_t)3;          /* prepare 3^(i+1) for next iteration */
    }
    return sum;
}
