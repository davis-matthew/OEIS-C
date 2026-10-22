/*
 * A318679  Sum of abundancies of abundant divisors of n.
 * https://oeis.org/A318679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, sigma(d)-2*d > 0} (sigma(d)-2*d)
 * Range:     u8 n <= 178, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sigma(d) is the sum of divisors of d; the summand is the
 *   “abundancy” of d, counted only when it is positive.       */

#include <stdlib.h>

/* Cache for σ(i) for i = 0 .. n (σ(0) is unused)                     */
static seq_t *sigma_cache = NULL;   /* sigma_cache[i] = σ(i) */
static idx_t   sigma_cap   = 0;     /* number of precomputed entries (>=0) */

/* Ensure that sigma_cache[1..n] contains σ(i)                         */
static void
ensure_sigma_upto(idx_t n)
{
    if (sigma_cap > n)
        return;                     /* already have enough entries    */

    /* Allocate space for indices 0..n (size n+1)                       */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(sigma_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                    /* out of memory                  */
    sigma_cache = tmp;
    sigma_cap   = new_cap;

    /* Initialise all to zero                                         */
    for (idx_t i = 0; i < new_cap; ++i)
        sigma_cache[i] = 0;

    /* Compute σ(i) by the classical divisor summing sieve            */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sigma_cache[j] += (seq_t)i;
}

/* Main sequence function                                             */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                /* sequence starts at n = 1            */

    /* Pre‑compute σ(d) for all d ≤ n                                 */
    ensure_sigma_upto(n);

    seq_t sum = 0;

    /* Loop over divisors of n by iterating up to √n                   */
    idx_t sqrt_n = 1;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        ++sqrt_n;

    for (idx_t i = 1; i <= sqrt_n; ++i)
    {
        if (n % i == 0)
        {
            /* d1 = i                                                      */
            seq_t twice = (seq_t)i * 2;
            if (sigma_cache[i] > twice)
                sum += sigma_cache[i] - twice;

            /* d2 = n / i                                                  */
            idx_t d2 = n / i;
            if (d2 != i)
            {
                twice = (seq_t)d2 * 2;
                if (sigma_cache[d2] > twice)
                    sum += sigma_cache[d2] - twice;
            }
        }
    }

    return sum;
}
