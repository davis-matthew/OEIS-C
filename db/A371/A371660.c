/*
 * A371660  G.f. satisfies A(x) = 1 + x * A(x)^3 * (1 + A(x) + A(x)^2).
 * https://oeis.org/A371660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=0..floor((n-1)/2)} 3^(n-k) * binomial(n,k)
 *            * binomial(4*n-k, n-1-2*k) for n > 0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t jmax = (n - 1) / 2;      /* floor((n-1)/2) */

    seq_t sum = 0;
    seq_t pow = pow3(n);           /* 3^n */
    seq_t binom_nk = 1;            /* binomial(n,0) */
    seq_t B = binomial(4 * n, n - 1); /* binomial(4n, n-1) for k=0 */

    for (idx_t k = 0; k <= jmax; ++k) {
        /* term = 3^(n-k) * binomial(n,k) * binomial(4*n-k, n-1-2*k) */
        seq_t term = pow * binom_nk * B;
        sum += term;

        if (k == jmax)
            break; /* finished */

        /* Update for k+1 */
        pow /= 3;                             /* 3^(n-(k+1)) */

        /* binomial(n,k+1) = binomial(n,k) * (n-k) / (k+1) */
        binom_nk = (binom_nk * (n - k)) / (k + 1);

        /* Update B: B_{k+1} = B_k * (B_k*(B_k-1)) / ((4*n-k)*(3*n+k+2)) */
        idx_t B_val = n - 1 - 2 * k;
        seq_t numerator = B * B_val * (B_val - 1);
        seq_t denom = (4 * n - k) * (3 * n + k + 2);
        B = numerator / denom;
    }

    /* a(n) = sum / n  (n > 0) */
    return sum / n;
}
