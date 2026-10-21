/*
 * A292499  a(n) equals the sum of the Gaussian binomial coefficients [n,k] at
 *          q=n for n>=0.
 * https://oeis.org/A292499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^n [n,k]_q evaluated at q = n where [n,k]_q =
 *            ∏_{i=1}^k (q^{n-i+1}-1)/(q^i-1) (Gaussian binomial)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* convert n to the integer type used in the sequence */
    seq_t n_t = (seq_t)n;

    /* pre‑compute powers of n: pow[i] = n^i */
    seq_t *pow = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow) return 0;                 /* allocation failure – unlikely in tests */
    pow[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow[i] = pow[i - 1] * n_t;      /* works for built‑in or arbitrary‑precision */

    /* G(1) = [n,1]_n = (n^n - 1)/(n - 1) */
    seq_t g = (pow[n] - 1) / (pow[1] - 1);
    seq_t sum = 2;                     /* accounts for G(0)=G(n)=1 */

    sum += g;                          /* add G(1) */

    /* compute G(m) for m = 2 .. n-1 iteratively */
    for (idx_t m = 1; m < n - 1; ++m) {
        /* update to G(m+1) */
        g = g * (pow[n - m] - 1);      /* multiply by (n^{n-m} - 1) */
        g = g / (pow[m + 1] - 1);      /* divide by (n^{m+1} - 1) */

        sum += g;                      /* add G(m+1) */
    }

    free(pow);
    return sum;
}
