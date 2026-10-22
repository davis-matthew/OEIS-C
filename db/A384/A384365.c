/*
 * A384365  a(n) = Sum_{k=0..n} (k+1) * 3^k * binomial(2*n+1,n-k).
 * https://oeis.org/A384365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 3^k * binomial(2*n+1, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 early for clarity */
    if (n == 0) return 1;

    /* Constant values for the binomial computation */
    const idx_t N = 2 * n + 1;           /* N = 2n+1 */
    idx_t r = n;                         /* Current r = n-k */

    /* Compute binomial(N, r) using multiplicative formula.
       binom = C(N, r) = ∏_{i=1..r} (N - r + i) / i .
       Here N - r = n+1, so N - r + i = n+1 + i.            */
    seq_t binom = 1;
    for (idx_t i = 1; i <= r; ++i) {
        binom = binom * (seq_t)(n + 1 + i);
        binom = binom / (seq_t)i;
    }

    seq_t pow3 = 1;      /* 3^k */
    seq_t sum  = 0;     /* Accumulator for the sum */

    for (idx_t k = 0; k <= n; ++k) {
        /* Add current term: (k+1) * 3^k * binomial(N, n-k) */
        sum += (seq_t)(k + 1) * pow3 * binom;

        if (k == n) break;   /* No further terms */

        /* Update binom to C(N, r-1):
           C(N, r-1) = C(N, r) * r / (N - r + 1)          */
        binom = binom * (seq_t)r / (seq_t)(N - r + 1);
        --r;                      /* r now equals n-(k+1)      */
        pow3 = pow3 * (seq_t)3;   /* 3^(k+1)                  */
    }

    return sum;
}
