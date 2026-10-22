/*
 * A383606  Expansion of 1/( (1-x) * (1-x-9*x^3)^2 )^(1/3).
 * https://oeis.org/A383606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (-9)^k * binomial(-2/3,k) *
 *            binomial(n-2*k,k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxk  = n / 3;
    seq_t f      = 1;   /* f(k) = 3^k * prod_{j=0}^{k-1}(3j+2) / k!  */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* binomial(n-2k, k)  */
        seq_t bin = 1;
        if (k > 0)
        {
            idx_t a = n - 2 * k;          /* a >= k   */
            for (idx_t i = 1; i <= k; ++i)
            {
                bin *= (a - i + 1);
                bin /= i;                 /* exact division */
            }
        }
        total += f * bin;

        /* update f for next k (if any) */
        if (k < maxk)
        {
            /* f(k+1) = f(k) * [3 * (3(k+1)-1)] / (k+1) = f(k) * (9*k+6) / (k+1) */
            seq_t mult = (seq_t)(9 * k + 6);
            f = f * mult;
            f = f / (k + 1);
        }
    }
    return total;
}
