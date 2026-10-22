/*
 * A356338  a(n) = Sum_{k=1..n} binomial(2*n, n-k) * sigma(k).
 * https://oeis.org/A356338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, n-k) * sigma(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes sigma(k) for k up to the largest n seen so
   far.  For each n it builds the binomial coefficients of (2*n choose j)
   incrementally, multiplying by (2*n - j)/(j+1) to avoid factorials.
   It works for arbitrary‐precision seq_t (e.g. unsigned __int128 or a
   big‑integer class) as well as for uint64_t. */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are defined by the build; we rely only on the
   basic arithmetic operators and the functions listed in the
   problem statement. */

static seq_t *sigma_cache = NULL;
static idx_t  sigma_size = 0;

/* Recompute sigma(k) for all 1 <= k <= n.  The array is resized if
   necessary.  Complexity is O(n log n) and is cheap for n <= 10 000. */
static void recompute_sigma(idx_t n)
{
    if (n + 1 <= sigma_size) return;         /* already computed up to n */

    /* Allocate new array */
    seq_t *newa = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!newa) abort();                      /* out of memory */

    /* Initialise: sigma[0] = 0, sigma[1..n] = 1 (divisor 1) */
    newa[0] = 0;
    for (idx_t i = 1; i <= n; ++i) newa[i] = 1;

    /* Add contributions of each divisor i(>=2).  This is the classic
       divisor–sum sieve. */
    for (idx_t i = 2; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            newa[j] += (seq_t)i;
        }
    }

    /* Replace old cache */
    free(sigma_cache);
    sigma_cache = newa;
    sigma_size   = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* Not defined for n=0 */

    /* Ensure sigma(k) is available for k <= n. */
    recompute_sigma(n);

    seq_t result = 0;
    seq_t bino   = 1;                     /* binomial(2*n, 0) */

    /* Incrementally compute binomial coefficients for j = 0 .. n-1
       and accumulate the weighted sum using sigma[n-j]. */
    for (idx_t j = 0; j <= n - 1; ++j) {
        /* contribution = bino * sigma[n-j] */
        result += bino * sigma_cache[n - j];

        /* Prepare next binomial term, except after the last iteration. */
        if (j < n - 1) {
            /* binomial(2*n, j+1) = binomial(2*n, j) * (2*n - j) / (j+1) */
            bino = bino * ((seq_t)(2 * n - j)) / ((seq_t)(j + 1));
        }
    }
    return result;
}
