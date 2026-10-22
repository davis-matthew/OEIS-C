/*
 * A338750  a(n) = 1 + Sum_{k=1..n-1} a(gcd(n,k)).
 * https://oeis.org/A338750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{n-1} a(gcd(n,k)) Recurrence used: a(n) = 1 +
 *            Σ_{d|n; d<n} φ(n/d) * a(d) (φ = Euler totient) Initial: a(1) = 1
 * Range:     u8 n <= 47, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoization state ---------------------------------------------------- */
static seq_t *cache = NULL;          /* cache[n] = a(n) for 1 ≤ n ≤ computed_max */
static size_t cache_cap = 0;          /* allocated size of cache (≥ computed_max+1) */
static idx_t   computed_max = 0;      /* maximum n for which cache[n] is ready */

/* Totient table state --------------------------------------------------- */
static seq_t *phi = NULL;            /* phi[i] = φ(i) for 1 ≤ i < phi_cap */
static size_t phi_cap = 0;            /* allocated size of phi (≥ current need) */

/*--------------------------------------------------------------------*/
/* Ensure phi[0..n] are valid. Rebuild the entire table as needed.    */
/*--------------------------------------------------------------------*/
static void ensure_phi(idx_t n)
{
    if ((size_t)n < phi_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_phi = (seq_t *)realloc(phi, new_cap * sizeof(seq_t));
    if (!new_phi) exit(1);
    phi = new_phi;

    for (size_t i = 0; i < new_cap; ++i)
        phi[i] = (seq_t)i;           /* will adjust below */

    phi[0] = 0;
    if (new_cap > 1)
        phi[1] = 1;

    for (size_t p = 2; p < new_cap; ++p) {
        if (phi[p] == (seq_t)p) {        /* p is prime */
            for (size_t j = p; j < new_cap; j += p)
                phi[j] -= phi[j] / (seq_t)p;
        }
    }
    phi_cap = new_cap;
}

/*--------------------------------------------------------------------*/
/* Ensure cache[0..n] is allocated.                                  */
/*--------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/*--------------------------------------------------------------------*/
/* Compute a(n) with memoization.                                     */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* out of domain, not used */

    ensure_cache(n);
    if (computed_max >= n)
        return cache[n];

    ensure_phi(n);

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
            continue;
        }

        seq_t val = 1;  /* the initial +1 in the recurrence */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                if (d < i) {
                    val += phi[(idx_t)(i / d)] * cache[d];
                }
                idx_t other = i / d;
                if (other != d && other < i) {
                    val += phi[(idx_t)(i / other)] * cache[other];
                }
            }
        }
        cache[i] = val;
    }

    computed_max = n;
    return cache[n];
}
