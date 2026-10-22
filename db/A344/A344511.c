/*
 * A344511  a(n) = Sum_{k >= 0} sign(d_k) * 2^k for any number n with decimal
 *          expansion Sum_{k >= 0} d_k * 10^k.
 * https://oeis.org/A344511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k >= 0} ( (d_k != 0) ? 1 : 0 ) * 2^k where n =
 *            Sum_{k >= 0} d_k * 10^k (decimal digits d_k)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans  = 0;   /* result accumulator */
    seq_t bit  = 1;   /* 2^k, starting with k = 0 */
    idx_t m = n;      /* working copy of n */

    while (m > 0) {
        unsigned digit = m % 10;   /* least‑significant decimal digit */
        if (digit != 0)
            ans |= bit;            /* set bit k if digit is non‑zero */
        m /= 10;                   /* shift to next digit */
        bit <<= 1;                 /* advance to next power of two */
    }
    return ans;
}
