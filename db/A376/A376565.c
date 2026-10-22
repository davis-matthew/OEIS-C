/*
 * A376565  E.g.f. satisfies A(x) = exp( x*A(x) * (1 + x^3*A(x)^3) ).
 * https://oeis.org/A376565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/4⌋} (n+1)^(n-3k-1) *
 *            binomial(n-3k,k)/(n-3k)! (Simplified: a(n) = Σ_{k} n!/(k!
 *            (n-4k)!) * (n+1)^(n-3k-1) )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"      /* seq_pow(seq_t base, idx_t exp) */

static seq_t *fac = NULL;          /* factorial table, fac[i] = i!   */
static idx_t fac_size = 0;         /* current size of fac (fac_size-1 is max n computed) */

/* Recompute factorial table up to n (inclusive).  Memory for old table is freed. */
static void ensure_factorial(idx_t n)
{
    if (n + 1 > fac_size) {
        seq_t *newfac = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!newfac) return;      /* out of memory – caller will handle failure */
        newfac[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            newfac[i] = newfac[i - 1] * (seq_t)i;
        free(fac);
        fac = newfac;
        fac_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_factorial(n);
    seq_t fact_n = fac[n];

    seq_t base   = (seq_t)(n + 1);
    seq_t power  = seq_pow(base, (idx_t)(n - 1));     /* (n+1)^(n-1) for k = 0 */
    seq_t base_cub = base * base * base;             /* (n+1)^3  */

    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* denom = k! * (n-4k)!  */
        seq_t denom = fac[k] * fac[n - 4 * k];
        seq_t term = fact_n / denom;          /* exact division */
        term = term * power;
        sum += term;

        if (k < maxk)                       /* prepare power for next k */
            power /= base_cub;               /* exact division by (n+1)^3 */
    }
    return sum;
}
