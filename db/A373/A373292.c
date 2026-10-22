/*
 * A373292  Expansion of 1/(1 - x/(1 - 8*x^3)^(1/4)).
 * https://oeis.org/A373292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 8^k * binomial((n+k)/4-1, k)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial(r,k) for rational r is defined by r*(r-1)*...*(r-k+1)/k!   */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;               /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* n + k - 4  is the integer a used in the product */
        seq_t a_val = (seq_t)n + (seq_t)k - 4;
        seq_t term  = 1;               /* term for this k */

        /* compute T(n,k) = (2^k / k!) * product_{j=0}^{k-1} (a_val - 4*j) */
        for (idx_t j = 0; j < k; ++j) {
            /* multiply by (a_val - 4*j) */
            term *= (a_val - 4 * j);

            /* multiply by 2 (the 2^k factor, one 2 per step) */
            term *= (seq_t)2;

            /* divide by (j+1) to handle the factorial; division is exact */
            term /= (j + 1);
        }

        sum += term;
    }

    return sum;
}
