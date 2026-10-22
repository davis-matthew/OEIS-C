/*
 * A338234  Sum of the numbers less than n whose square does not divide n.
 * https://oeis.org/A338234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) =  ∑_{k=1}^{n-1} k  –  ∑_{k: k²|n} k
 * The first sum is n·(n‑1)/2. The second sum is multiplicative:
 * if n = ∏ p_i^{e_i} then
 *   ∑_{k: k²|n} k = ∏_i ( 1 + p_i + p_i² + … + p_i^{⌊e_i/2⌋} )
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* total sum of all numbers < n */
    seq_t N = (seq_t)n;
    seq_t total = N * (N - 1) / 2;

    /* compute sum of k such that k² divides n */
    idx_t tmp = n;
    seq_t sum_sqdiv = 1;          /* multiplicative identity */

    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p != 0) continue;

        idx_t exp = 0;
        while (tmp % p == 0)      /* count exponent of p in n */
        {
            tmp /= p;
            ++exp;
        }

        idx_t half = exp / 2;     /* largest exponent for k  (k² | n)  */
        seq_t sum_f = 1;          /* p^0 contributes 1 */
        seq_t p_pow = 1;

        for (idx_t f = 1; f <= half; ++f)
        {
            p_pow *= (seq_t)p;    /* multiply by p to get p^f */
            sum_f += p_pow;      /* accumulate 1 + p + p² + … + p^half */
        }

        sum_sqdiv *= sum_f;        /* multiplicative combination of primes */
    }

    /* if tmp > 1, it is a remaining prime factor with exponent 1,
       contributing only 1 to the product, so nothing to do. */

    return total - sum_sqdiv;
}
