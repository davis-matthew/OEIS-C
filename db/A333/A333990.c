/*
 * A333990  a(n) = Sum_{k=0..n} n^k * binomial(2*n,2*k).
 * https://oeis.org/A333990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(2*n, 2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;          /* n^0 */
    seq_t comb = 1;         /* binomial(2*n, 0) */
    idx_t two_n = 2 * n;

    for (idx_t k = 0; k <= n; ++k) {
        /* accumulate term for current k */
        sum += pow * comb;

        if (k == n) break;          /* no further terms */

        /* advance n^k → n^{k+1} */
        pow *= (seq_t)n;

        /* advance binomial C(2n, 2k) → C(2n, 2k+2) */
        idx_t mul1_idx = two_n - 2 * k;          /* 2n – 2k   */
        idx_t mul2_idx = two_n - 2 * k - 1;      /* 2n – 2k-1 */
        seq_t mul1 = (seq_t)mul1_idx;
        seq_t mul2 = (seq_t)mul2_idx;

        comb = comb * mul1 * mul2;

        idx_t denom1 = 2 * k + 2;     /* 2k + 2 */
        idx_t denom2 = 2 * k + 1;     /* 2k + 1 */
        comb /= ((seq_t)denom1 * (seq_t)denom2);
    }

    return sum;
}
