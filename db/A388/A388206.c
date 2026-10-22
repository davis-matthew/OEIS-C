/*
 * A388206  a(n) = Sum_{k=0..n} 4^k * binomial(n,k) * binomial(n+3,k).
 * https://oeis.org/A388206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(n,k) * binomial(n+3,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;     /* accumulated result */
    seq_t power = 1;     /* current value of 4^k */
    seq_t binom1 = 1;    /* binomial(n,  k)  */
    seq_t binom2 = 1;    /* binomial(n+3, k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 4^k * binom(n,k) * binom(n+3,k) */
        seq_t term = power;
        term = term * binom1;
        term = term * binom2;
        sum += term;

        if (k == n) break;

        /* Advance to next k:
         *   power   ← 4^(k+1)
         *   binom1  ← binomial(n, k+1) = binom1 * (n-k) / (k+1)
         *   binom2  ← binomial(n+3, k+1) = binom2 * (n+3-k) / (k+1)
         */
        power = power * (seq_t)4;

        seq_t tmp = binom1 * (n - k);
        binom1 = tmp / (k + 1);

        tmp = binom2 * (n + 3 - k);
        binom2 = tmp / (k + 1);
    }

    return sum;
}
