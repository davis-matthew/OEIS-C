/*
 * A309730  Expansion of Sum_{k>=1} x^k * (1 - x^(3*k))/(1 - x^k)^4.
 * https://oeis.org/A309730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * (sigma_2(n) - sigma_1(n))/2 + d(n)
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sigma_k(n)   : sum of the k-th powers of the divisors of n   */
/*  d(n)         : number of divisors of n                       */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but guard against 0 */

    seq_t sum1 = 0;   /* sigma_1(n) */
    seq_t sum2 = 0;   /* sigma_2(n) */
    seq_t divcnt = 0; /* d(n) */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;

            /* add divisor i */
            seq_t si = (seq_t)i;
            sum1 += si;
            sum2 += si * si;
            ++divcnt;

            if (j != i) {
                seq_t sj = (seq_t)j;
                sum1 += sj;
                sum2 += sj * sj;
                ++divcnt;
            }
        }
    }

    seq_t diff = sum2 - sum1;          /* sigma_2(n) - sigma_1(n) */
    seq_t tmp  = (diff / 2) * 3;       /* 3 * (diff)/2  */
    return tmp + divcnt;
}
