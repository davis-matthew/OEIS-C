/*
 * A267326  Number of ways writing n^2 as a sum of four squares: a(n) =
 *          A000118(n^2).
 * https://oeis.org/A267326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*σ(n²) if n is odd * a(n) = 24*σ(m²) if n is even, where
 *            m is the largest odd divisor of n * σ(k) is the sum of divisors
 *            of k. a(0)=1
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 5293, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;                      /* n² = 0 has only the all‑zero representation */

    idx_t orig_n = n;                          /* keep original n for parity test   */
    idx_t m = n;                                /* m will become the largest odd divisor  */
    while ((m & 1) == 0) m >>= 1;              /* strip all factors of 2               */

    /* compute σ(m²) multiplicatively */
    seq_t sigma = 1;
    idx_t tmp = m;                            /* temporary copy for factorisation     */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {          /* count exponent of p in m            */
                tmp /= p;
                ++e;
            }
            /* term = (p^(2e+1) - 1) / (p - 1) */
            seq_t num = 1;
            idx_t exp = 2 * e + 1;
            for (idx_t i = 0; i < exp; ++i) num *= (seq_t)p;
            seq_t denom = (seq_t)(p - 1);
            seq_t term = (num - 1) / denom;
            sigma *= term;
        }
    }
    if (tmp > 1) {                           /* remaining prime factor (exponent 1)   */
        seq_t num = (seq_t)tmp * tmp * tmp;   /* p^3                                 */
        seq_t denom = (seq_t)(tmp - 1);
        seq_t term = (num - 1) / denom;
        sigma *= term;
    }

    /* final result, multiply by 8 or 24 depending on parity of original n */
    if (orig_n & 1)                           /* odd n */
        return sigma * 8;
    else
        return sigma * 24;
}
