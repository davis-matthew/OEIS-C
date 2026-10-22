/*
 * A343761  a(1) = 1; a(n) = -Sum_{k=1..n, gcd(n,k) > 1} a(n/gcd(n,k)).
 * https://oeis.org/A343761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d < n} φ(d) * a(d)
 * Range:     i8 n <= 51, i16 n <= 159, i32 n <= 831, i64 n <= 8703,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation memoises values and pre‑computes φ up to the
    largest n requested.  It works for any seq_t type (fixed‑size or
    arbitrary‑precision) and for any idx_t that is a 64‑bit unsigned type. */

#include <stdlib.h>
#include <stdbool.h>

/* --- helper for allocating/ensuring cache (from utils) --- */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    /* binary grow strategy (simple one‑off allocation) */
    if (*cache_ptr == NULL) {
        *cache_ptr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!*cache_ptr) exit(1);
        for (idx_t i = 0; i <= n; ++i) (*cache_ptr)[i] = 0;
        *cap_ptr = n + 1;
    } else if (*cap_ptr <= n) {
        *cache_ptr = (seq_t*)realloc(*cache_ptr, (size_t)(n + 1) * sizeof(seq_t));
        if (!*cache_ptr) exit(1);
        for (idx_t i = *cap_ptr; i <= n; ++i) (*cache_ptr)[i] = 0;
        *cap_ptr = n + 1;
    }
}

/* --- φ sieve up to N (uint64_t array) --- */
static uint64_t *phi = NULL;
static size_t phi_cap = 0;

static void compute_phi_upto(idx_t N)
{
    /* Allocate/resize */
    phi = (uint64_t*)realloc(phi, (size_t)(N + 1) * sizeof(uint64_t));
    if (!phi) exit(1);
    for (idx_t i = 0; i <= N; ++i) phi[i] = i;
    for (idx_t p = 2; p <= N; ++p) {
        if (phi[p] == p) {                    /* p is prime */
            for (idx_t j = p; j <= N; j += p) {
                phi[j] -= phi[j] / p;
            }
        }
    }
    phi_cap = (size_t)(N + 1);
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence starts at offset 1 */

    /* Static state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* Ensure cache can hold up to n */
    ensure_cache(n, &cache, &cache_cap);
    /* Ensure φ values up to n are ready */
    if (phi_cap <= (size_t)n) compute_phi_upto(n);

    /* Compute new terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t other = i / d;
                if (d < i) {
                    sum += (seq_t)phi[d] * cache[d];
                }
                if (other != d && other < i) {
                    sum += (seq_t)phi[other] * cache[other];
                }
            }
        }
        cache[i] = -sum;
    }
    max_computed = (idx_t) (max_computed < n ? n : max_computed);
    return cache[n];
}
