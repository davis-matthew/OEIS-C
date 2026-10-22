/*
 * A361700  Constant term in the expansion of (1 + x^4 + y^4 + 1/(x*y))^n.
 * https://oeis.org/A361700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/6)} binomial(2*k,k) * binomial(6*k,2*k)
 *            * binomial(n,6*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Local helper to compute binomial coefficients exactly in seq_t. */
static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        /* Multiply first then divide – the division is exact for binomials. */
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t kmax = n / (idx_t)6;
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= kmax; ++k)
    {
        seq_t t1 = my_binom((idx_t)(2*k),   k);   /* binomial(2k,k)   */
        seq_t t2 = my_binom((idx_t)(6*k), (idx_t)(2*k)); /* binomial(6k,2k) */
        seq_t t3 = my_binom(n, (idx_t)(6*k)); /* binomial(n,6k)   */
        seq_t term = t1 * t2 * t3;
        sum += term;
    }
    return sum;
}
