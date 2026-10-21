/*
 * A244328  a(1) = a(2) = 0; for n >= 3: a(n) = floor((n*(n+1)/2) /
 *          antisigma(n)) = floor(A000217(n) / A024816(n)).
 * https://oeis.org/A244328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A244328
 *  a(n) = floor( A000217(n) / A024816(n) )
 *  where A000217(n) = n*(n+1)/2 (triangular number)
 *  and A024816(n) = A000217(n) – σ(n)   (σ = sum of positive divisors)
 *
 *  The function caches divisor sums up to the largest n requested, 
 *  computing them by a simple sieve.
 */

#include <stdlib.h>

static seq_t *sigma_array = NULL;   /* σ values, 1‑based (sigma_array[0] unused) */
static idx_t sigma_limit = 0;       /* largest n for which sigma_array is filled */

/* Ensure σ values are computed for all 1 ≤ i ≤ limit. */
static void ensure_sigma(idx_t limit)
{
    if (limit <= sigma_limit)
        return;

    /* Allocate a new array large enough for indices 0..limit. */
    seq_t *new_sigma = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_sigma)          /* allocation failed – caller will handle failure */
        return;

    /* Initialise to zero. */
    for (idx_t i = 0; i <= limit; ++i)
        new_sigma[i] = 0;

    /* Sieve: add each i to all multiples of i. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i)
            new_sigma[j] += (seq_t)i;
    }

    /* Free old array if it exists. */
    if (sigma_array)
        free(sigma_array);

    sigma_array = new_sigma;
    sigma_limit = limit;
}

/* Sequence implementation */
seq_t a(idx_t n)
{
    if (n <= 2) /* defined as 0 for n = 1,2 */
        return 0;

    /* Compute divisor sums up to n, if necessary. */
    ensure_sigma(n);

    /* Triangular number T = n*(n+1)/2, computed in seq_t. */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Denominator: antisigma = T – σ(n). */
    seq_t denom = T - sigma_array[n];
    if (denom == 0)    /* safety, though this only happens for n = 1,2 */
        return 0;

    /* Floor division. */
    return T / denom;
}
