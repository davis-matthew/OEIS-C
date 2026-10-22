/*
 * A374614  a(n) = Sum_{k=0..n} (k/n)^2 * binomial(n,k)^5.
 * https://oeis.org/A374614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n-1,k)^2 * binomial(n,k)^3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, offset starts at 1 */

    seq_t sum = 0;

    /* Iteratively compute binomial(n-1,k) and binomial(n,k) */
    seq_t b1 = 1;      /* binomial(n-1,0) */
    seq_t b2 = 1;      /* binomial(n,0)   */
    idx_t N1 = n - 1;  /* for first binomial row */
    idx_t N2 = n;      /* for second binomial row */

    for (idx_t k = 0; k < n; ++k) {
        /* term = b1^2 * b2^3 */
        seq_t term = b1 * b1;
        seq_t b2_sq = b2 * b2;
        seq_t b2_cu = b2_sq * b2;
        term = term * b2_cu;
        sum += term;

        if (k + 1 < n) {
            /* update b1 for k+1 : b1 = b1 * (N1 - k) / (k+1) */
            seq_t tmp = b1 * (seq_t)(N1 - k);
            b1 = tmp / (seq_t)(k + 1);

            /* update b2 for k+1 : b2 = b2 * (N2 - k) / (k+1) */
            tmp = b2 * (seq_t)(N2 - k);
            b2 = tmp / (seq_t)(k + 1);
        }
    }
    return sum;
}
