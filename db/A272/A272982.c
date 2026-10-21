/*
 * A272982  a(n) is the number of ways of putting n labeled balls into 3
 *          indistinguishable boxes such that each box contains at least 3
 *          balls.
 * https://oeis.org/A272982
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + C(n-1,2)*(2^(n-4)+2-n-C(n-3,2)), a(n) = 0 for
 *            n < 9
 * Range:     u8 none, u16 n <= 12, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9)
        return 0;

    seq_t val = 280;          /* a(9) */
    seq_t pow2_m4 = 32;       /* 2^(9-4) */

    for (idx_t i = 10; i <= n; ++i) {
        /* Update power of two: 2^(i-4) */
        pow2_m4 = pow2_m4 * 2;

        /* Binomial coefficients */
        seq_t binom1 = ((seq_t)(i - 1) * (seq_t)(i - 2)) / 2;  /* C(i-1,2) */
        seq_t binom2 = ((seq_t)(i - 3) * (seq_t)(i - 4)) / 2;  /* C(i-3,2) */

        /* Compute term and update a(i) */
        seq_t temp = pow2_m4 + (seq_t)2 - (seq_t)i - binom2;
        val = 3 * val + binom1 * temp;
    }
    return val;
}
