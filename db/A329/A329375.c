/*
 * A329375  a(n) = Sum_{d|n, d<n} d*omega(d).
 * https://oeis.org/A329375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} d * A001221(d) (omega(d) = number of
 *            distinct prime factors)
 * Range:     u8 n <= 83, u16 n <= 7139, u32 n <= 9394, u64 n <= 9394,
 *            u128 n <= 9394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Size of the precalculated arrays */
static idx_t *spf = NULL;      /* smallest prime factor */
static seq_t *omega = NULL;    /* number of distinct prime factors */
static idx_t spf_cap = 0;      /* current capacity of spf and omega arrays */

/* Ensure spf and omega are computed for all numbers up to n (inclusive). */
static void _ensure_spf_omega(idx_t n)
{
    if (n + 1 <= spf_cap)
        return;

    /* free old arrays */
    if (spf)
        free((void *)spf);
    if (omega)
        free((void *)omega);

    /* allocate new arrays of size n+1 (indices 0..n) */
    spf    = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    omega  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!spf || !omega)
        return; /* out of memory; caller will receive 0 */

    /* simple sieve for smallest prime factor */
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;                /* initialize: spf[i] = i (prime for now) */
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (spf[p] != p)   /* not prime */
            continue;
        for (idx_t m = p * p; m <= n; m += p)
        {
            if (spf[m] == m)
                spf[m] = p;
        }
    }

    /* compute omega values */
    omega[0] = 0;
    omega[1] = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t p = spf[i];
        idx_t k = i / p;
        if (k % p == 0)
            omega[i] = omega[k];
        else
            omega[i] = omega[k] + 1;
    }

    spf_cap = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;                /* a(1) = 0 */

    /* Precompute omega up to n */
    _ensure_spf_omega(n);

    /* sum over divisors d of n with d < n */
    seq_t sum = 0;
    /* iterate over all potential divisors up to sqrt(n) for efficiency,
       but since n <= 10^4 this simple loop is fine */
    for (idx_t d = 1; d < n; ++d)
    {
        if (n % d == 0)
        {
            sum += (seq_t)d * omega[d];
        }
    }

    return sum;
}
