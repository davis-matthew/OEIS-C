/*
 * A385326  The number of positive k <= 2*n + 1 such that 2*n + 1 divides (2^k
 *          + 2*n + 1)^2 - 1.
 * https://oeis.org/A385326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k ∈ [1..2n+1] : (2n+1) | (2^k + 2n+1)^2 – 1}
 *            Equivalent: a(n) = ⌊(2n+1)/ord_{2n+1}(4)⌋, where ord_m(4) is the
 *            multiplicative order of 4 modulo m (m odd)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  M = 2n + 1 is always odd */
    idx_t M = 2*n + 1;

    /* Special case:  M = 1  (n = 0) */
    if (M == 1) return 1;

    /* Compute the multiplicative order of 4 modulo M */
    seq_t mod = (seq_t)M;          /* used as the modulus in calculations */
    seq_t pow = 4 % mod;           /* 4^1 (mod M) */
    idx_t    d   = 1;              /* current exponent  (k) */

    while (pow != 1) {
        pow = (pow * 4) % mod;     /* advance to 4^{k+1} (mod M) */
        d++;
    }

    /* The count is floor(M / d) */
    return (seq_t)(M / d);
}
