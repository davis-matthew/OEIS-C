/*
 * A274671  Diagonal of the rational function 1/(1 - x - y - z - x y + x z + y
 *          z - x y z).
 * https://oeis.org/A274671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n+k,k) * 2^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* running sum of terms */
    seq_t c     = 1;          /* binomial(n,0) */
    seq_t d     = 1;          /* binomial(n+0,0) = 1 */
    seq_t pow2  = 1;          /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = c^2 * d * 2^k */
        seq_t term = c * c;
        term = term * d;
        term = term * pow2;
        sum += term;

        if (k == n) break;    /* avoid overflow on last iteration */

        /* update binomial coefficients for next k+1 */
        seq_t denom = (seq_t)(k + 1);
        c = (c * (seq_t)(n - k)) / denom;
        d = (d * (seq_t)(n + k + 1)) / denom;

        /* next power of two */
        pow2 = pow2 * (seq_t)2;
    }
    return sum;
}
