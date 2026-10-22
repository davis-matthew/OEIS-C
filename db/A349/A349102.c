/*
 * A349102  Increase the odd part of n to the next greater odd number.
 * https://oeis.org/A349102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3; a(2n)=2*a(n); a(2n+1)=2n+3. Equivalent closed form: let
 *            n = 2^k * m with m odd, then a(n) = 2^k * (m + 2)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for non‑positive indices */

    idx_t m = n;          /* odd part */
    idx_t k = 0;          /* exponent of two */

    while ((m & 1u) == 0) {
        m >>= 1;
        ++k;
    }

    /* compute 2^k in idx_t (width at least 64) to avoid undefined shift on seq_t */
    idx_t power = (idx_t)1 << k;
    seq_t res = (seq_t)power * (((seq_t)m) + (seq_t)2);

    return res;
}
