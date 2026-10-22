/*
 * A386826  a(n) = Sum_{k=0..n} 3^(n-k)*C(2*n,n-k).
 * https://oeis.org/A386826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k)*C(2*n,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;   /* C(2*n,0) */
    seq_t power = 1;   /* 3^0   */

    for (idx_t j = 0; j <= n; ++j)
    {
        /* add term 3^j * C(2*n, j) */
        sum += binom * power;

        if (j == n) break;               /* last term processed */

        /* update binomial: C(2*n, j+1) = C(2*n, j) * (2*n - j) / (j+1) */
        seq_t numer = (seq_t)(2 * n - j);
        binom = (binom * numer) / (j + 1);

        /* update power of 3: 3^(j+1) = 3^j * 3 */
        power *= 3;
    }

    return sum;
}
