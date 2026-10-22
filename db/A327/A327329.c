/*
 * A327329  Twice the sum of all divisors of all positive integers <= n.
 * https://oeis.org/A327329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * sum_{k=1}^{n} sigma(k), * where sigma(k) is the sum
 *            of all positive divisors of k
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types, provided by the build system. */
/* idx_t   – unsigned integer type for indices (uint64_t)   */
/* seq_t   – unsigned integer type for sequence values     */

static seq_t *sigma      = NULL;   /* sigma[ i ] = sum of divisors of i   */
static seq_t *prefix     = NULL;   /* prefix[ i ] = sum_{k=1}^{i} sigma(k) */
static idx_t  max_alloc  = 0;      /* highest n for which arrays are ready */

/*
 * Helper that rebuilds the sigma and prefix tables for all
 * integers up to n (inclusive).  For simplicity this function
 * recomputes everything from scratch whenever a larger n is
 * requested.  Since n is expected to be at most ~10,000 the cost
 * is negligible (< 0.1 ms).
 */
static void rebuild_to(idx_t n)
{
    /* Allocate/resize both tables. */
    size_t sz = (size_t)(n + 1);
    sigma  = (seq_t *)realloc(sigma, sz * sizeof(seq_t));
    prefix = (seq_t *)realloc(prefix, sz * sizeof(seq_t));

    /* Initialize sigma array to zero. */
    for (idx_t i = 0; i <= n; ++i)
        sigma[i] = 0;

    /* Compute sigma using the harmonic‑sum sieve. */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t d_seq = (seq_t)d;
        for (idx_t m = d; m <= n; m += d)
            sigma[m] += d_seq;
    }

    /* Build the prefix sum table.  prefix[0] = 0 for convenience. */
    prefix[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        prefix[i] = prefix[i-1] + sigma[i];

    max_alloc = n;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0, return 0 for safety */

    if (n > max_alloc)
        rebuild_to(n);

    /* a(n) = 2 * Σ_{k=1}^{n} σ(k) = 2 * prefix[n] */
    return 2 * prefix[n];
}
