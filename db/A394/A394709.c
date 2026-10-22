/*
 * A394709  Coefficient of x^(2*n) in the expansion of ( (1+x)^3 * (1+x^2)
 *          )^n.
 * https://oeis.org/A394709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*n, n+2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i, k;
    seq_t sum = 0;
    seq_t C   = 1;                     /* C = binomial(n,0)           */
    seq_t B   = 1;                     /* B = binomial(3*n, n)        */

    /* Compute B = binomial(3*n, n) iteratively */
    for (i = 0; i < n; ++i) {
        B = B * (seq_t)(3 * n - i);
        B = B / (seq_t)(i + 1);
    }

    /* Main loop over k = 0 .. n */
    for (k = 0; k <= n; ++k) {
        /* add the current term */
        sum += C * B;

        if (k == n) break;   /* finished */

        /* Update C to binomial(n, k+1) */
        C = C * (seq_t)(n - k);
        C = C / (seq_t)(k + 1);

        /* Update B to binomial(3*n, n + 2*(k+1)) */
        /*   B_{k}  -> B_{k+1} via two one‑step recurrences   */
        idx_t m = n + 2 * k;          /* current m = n + 2k          */
        /* first step: multiply by (3n - m) / (m+1) = 2(n-k)/(m+1) */
        B = B * (seq_t)(2 * (n - k));
        B = B / (seq_t)(m + 1);
        /* second step: multiply by (3n - (m+1)) / (m+2) = (2(n-k)-1)/(m+2) */
        B = B * (seq_t)(2 * (n - k) - 1);
        B = B / (seq_t)(m + 2);
    }

    return sum;
}
