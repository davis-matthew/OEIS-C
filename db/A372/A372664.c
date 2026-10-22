/*
 * A372664  a(n) = Sum_{j=1..n} Sum_{k=1..n} phi(2*j*k) / phi(k).
 * https://oeis.org/A372664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{j=1..n} Sum_{k=1..n} phi(2*j*k) / phi(k)
 *
 * Implementation detail:
 *   Use the identity  phi(2*j*k)/phi(k) = phi(2*j) * gcd(2*j,k) / phi(gcd(2*j,k)).
 *   Thus only phi values up to 2*n are required.
 *   The sequence is generated incrementally: a(n) = a(n-1) + delta_n,
 *   where delta_n consists of the new row and column added when extending
 *   the double sum to size n.
 *   All computations use 64‑bit arithmetic; the results are accumulated
 *   in the generic seq_t type supplied by the build.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute gcd for unsigned 64‑bit values. */
static inline idx_t _gcd_u64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sieve of Euler to fill phi[0..limit] (inclusive). */
static uint64_t *phi = NULL;
static size_t    phi_lim = 0;   /* currently allocated size (elements) */

static void _ensure_phi(size_t limit)   /* limit inclusive */
{
    if (limit + 1 <= phi_lim)
        return;

    /* Allocate a new array and compute phi up to 'limit'. */
    uint64_t *new_phi = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_phi)
        abort();

    for (size_t i = 0; i <= limit; ++i)
        new_phi[i] = i;

    if (limit >= 1)
        new_phi[1] = 1;

    for (size_t p = 2; p <= limit; ++p) {
        if (new_phi[p] == p) {              /* p is prime */
            for (size_t m = p; m <= limit; m += p)
                new_phi[m] -= new_phi[m] / p;
        }
    }

    free(phi);
    phi = new_phi;
    phi_lim = limit + 1;
}

/* Cache of computed a(n) values. */
static seq_t *cache = NULL;
static idx_t  cache_n = 0;   /* highest n that has been cached */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* offset 1; values below are undefined */

    /* Ensure phi values up to 2*n are available. */
    _ensure_phi((size_t)(2 * n));

    /* Initialise cache if needed. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)0;
        cache_n  = 0;
    }

    /* Extend cache to n, computing intervening values. */
    if (n > cache_n) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;

        for (idx_t m = cache_n + 1; m <= n; ++m) {
            seq_t delta = 0;
            uint64_t phi_2m = phi[(size_t)(2 * m)];

            /* Sum over j = 1..m of phi(2*j)*gcd(2*j,m)/phi(gcd(2*j,m)) */
            for (idx_t j = 1; j <= m; ++j) {
                uint64_t phi_2j = phi[(size_t)(2 * j)];
                uint64_t g      = _gcd_u64((size_t)(2 * j), (size_t)m);
                uint64_t term   = phi_2j * g / phi[(size_t)g];
                delta += (seq_t)term;
            }

            /* Sum over k = 1..m-1 of phi(2*m)*gcd(2*m,k)/phi(gcd(2*m,k)) */
            for (idx_t k = 1; k < m; ++k) {
                uint64_t g    = _gcd_u64((size_t)(2 * m), (size_t)k);
                uint64_t term = phi_2m * g / phi[(size_t)g];
                delta += (seq_t)term;
            }

            cache[m] = cache[m - 1] + delta;
        }
        cache_n = n;
    }

    return cache[n];
}
