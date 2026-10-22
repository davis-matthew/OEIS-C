/*
 * A377832  Expansion of e.g.f. (1/x) * Series_Reversion( x * (1 - x)^2 *
 *          exp(-x) ).
 * https://oeis.org/A377832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (n+1)^(k-1) * binomial(3*n - k + 1, n -
 *            k) / k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer form: a(n) = Σ_{k=0..n} (n! / k!) * (n+1)^(k-1) * binomial(3*n - k + 1, n - k) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* binomial(3*n+1, n) */
    seq_t B = 1;
    for (idx_t i = 1; i <= n; ++i)
        B = B * (seq_t)(3 * n + 2 - i) / (seq_t)i;

    /* term for k = 0 : (n! / 0!) * (n+1)^(-1) * binomial(...)  */
    seq_t sum = fact * B / (seq_t)(n + 1);

    /* (n+1)^(k-1) for the current k, initialise for k = 1 as 1 */
    seq_t pow = 1;

    /* iterate k = 1 .. n using the recurrence for B and factorial factor */
    for (idx_t m = 0; m < n; ++m)          /* m = k-1 */
    {
        /* B = binomial(3*n - (m+1) + 1, n - (m+1)) from previous B */
        B = B * (seq_t)(n - m) / (seq_t)(3 * n - m + 1);

        /* factorial factor n! / k! where k = m+1 */
        fact = fact / (seq_t)(m + 1);

        /* term_k = (n! / k!) * (n+1)^(k-1) * binomial(...) */
        seq_t term = fact * pow * B;
        sum += term;

        /* update power for next k */
        pow *= (seq_t)(n + 1);
    }

    return sum;
}
