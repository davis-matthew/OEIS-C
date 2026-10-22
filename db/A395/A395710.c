/*
 * A395710  Expansion of g^6*(6*g-5)/(5-4*g)^4, where g = 1+x*g^5 is the g.f.
 *          of A002294.
 * https://oeis.org/A395710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * \sum_{k=0}^n 4^(n-k) * binomial(5*n+5,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 4^n : start at 1 and multiply by 4 n times. */
    seq_t pow   = 1;
    seq_t term  = 1;        /* binomial(5*n+5,0) */
    seq_t sum   = 0;

    for (idx_t i = 0; i < n; ++i) pow *= 4;

    for (idx_t k = 0; k <= n; ++k) {
        sum += term * pow;          /* add current term * 4^(n-k) */

        if (k == n) break;          /* no more terms afterwards */

        /* next binomial coefficient:
           binomial(5*n+5, k+1) = binomial(5*n+5, k) * (5*n+5-k)/(k+1)
        */
        seq_t numerator   = (seq_t)(5 * n + 5 - k);
        seq_t denominator = (seq_t)(k + 1);
        term = (term * numerator) / denominator;   /* exact division */

        pow /= 4;  /* 4^(n-(k+1)) from 4^(n-k) */
    }

    return (seq_t)(n + 1) * sum;
}
