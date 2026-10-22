/*
 * A375607  Expansion of e.g.f. 1 / (exp(-x^3) - x).
 * https://oeis.org/A375607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (n-3k+1)^k / k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum = 0;
    idx_t maxk = n / 3U;        /* floor(n/3)            */
    seq_t fact_div = fact_n;    /* holds n! / k! at each step */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t base  = (seq_t)(n - 3 * k + 1);
        seq_t power = 1;                  /* base^k                */
        for (idx_t j = 0; j < k; ++j)
            power *= base;

        sum += fact_div * power;

        if (k < maxk)
            fact_div /= (seq_t)(k + 1);   /* prepare for next k     */
    }
    return sum;
}
