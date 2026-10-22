/*
 * A330148  a(n) = Sum_{k=1..n} binomial(n,k) * phi(k) * phi(n - k + 1), where
 *          phi = A000010.
 * https://oeis.org/A330148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * phi(k) * phi(n-k+1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t _phi_precomputed = 0;
static seq_t *_phi_cache = NULL;

/* Ensure that _phi_cache has phi values for all indices up to n. */
static void _ensure_phi(idx_t n)
{
    if (n <= _phi_precomputed)
        return;

    seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failure – not expected for small n */

    /* initialise phi[i] = i */
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = (seq_t)i;

    /* compute phi by Euler's product formula using a sieve */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (new_cache[p] == (seq_t)p) /* p is prime */
        {
            for (idx_t j = p; j <= n; j += p)
                new_cache[j] -= new_cache[j] / (seq_t)p;
        }
    }

    free(_phi_cache);
    _phi_cache = new_cache;
    _phi_precomputed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* OEIS offset starts at 1, but guard against n==0 */

    _ensure_phi(n);     /* guarantee phi values up to n */

    seq_t total = 0;
    seq_t bin = 1;      /* binomial coefficient C(n,0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* C(n,k) = C(n,k-1) * (n-k+1)/k */
        bin = bin * (seq_t)(n - k + 1) / (seq_t)k;
        seq_t term = bin * _phi_cache[k] * _phi_cache[n - k + 1];
        total += term;
    }

    return total;
}
