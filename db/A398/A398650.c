/*
 * A398650  Number of composite numbers less than the n-th primorial that have
 *          a common factor greater than 1 with the (n-1)-th primorial.
 * https://oeis.org/A398650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * (P_{n-1} - φ(P_{n-1})) - (n-1) where P_k =
 *            product of the first k primes (primorial) and φ is Euler's
 *            totient function
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   build_primes_upto, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Utilities for primes and dynamic arrays */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ---------- Global state ---------- */
static uint64_t *prime_cache = NULL;       /* first primes, allocated by build_primes_upto */
static size_t   primes_len   = 0;          /* number of primes stored */

static seq_t *prim_cache   = NULL;        /* primorial P_k, size >= last computed */
static seq_t *phi_cache    = NULL;        /* φ(P_k)       , size >= last computed */
static size_t cache_len    = 0;            /* allocated length of prim_cache / phi_cache */
static idx_t  cached_to    = 0;            /* last index computed in the caches */

/* Ensure at least n primes are available (primes indexed from 1) */
static void ensure_primes(idx_t n)
{
    if (primes_len >= (size_t)n)
        return;

    /* upper bound for the nth prime: n(log n + log log n) + 3  (Rosser's theorem) */
    size_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = (size_t)((double)n * (log((double)n) + log(log((double)n))) + 3.0);

    uint64_t *tmp = NULL;
    size_t cnt   = 0;
    build_primes_upto((idx_t)limit, &tmp, &cnt);

    free(prime_cache);
    prime_cache = tmp;
    primes_len  = cnt;
}

/* Ensure the primorial and phi arrays can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n)
        return;

    size_t new_len = (size_t)n + 1;  /* include index 0 */
    seq_t *new_prim = seq_alloc_new_array(new_len);
    seq_t *new_phi  = seq_alloc_new_array(new_len);

    if (prim_cache != NULL) {
        memcpy(new_prim, prim_cache, cache_len * sizeof(seq_t));
        memcpy(new_phi,  phi_cache,  cache_len * sizeof(seq_t));
        seq_free_array(prim_cache);
        seq_free_array(phi_cache);
    }

    prim_cache = new_prim;
    phi_cache  = new_phi;
    cache_len  = new_len;

    /* initialise the zeroth entries if the arrays were freshly created */
    if (cached_to == 0) {
        prim_cache[0] = (seq_t)1;
        phi_cache[0]  = (seq_t)1;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* safety for indices below the offset */

    /* Prepare required data */
    ensure_primes(n);
    ensure_cache(n);

    /* Compute primorials and totients up to n if needed */
    for (idx_t k = cached_to + 1; k <= n; ++k) {
        seq_t p   = (seq_t)prime_cache[(size_t)k - 1];   /* k-th prime */
        prim_cache[(size_t)k] = prim_cache[(size_t)k - 1] * p;
        phi_cache[(size_t)k]  = phi_cache[(size_t)k - 1] * (p - (seq_t)1);
    }
    cached_to = n;

    /* a(n) = prime(n) * (P_{n-1} - φ(P_{n-1})) - (n-1) */
    seq_t pn   = (seq_t)prime_cache[(size_t)n - 1];
    seq_t diff = prim_cache[(size_t)n - 1] - phi_cache[(size_t)n - 1];
    seq_t res  = pn * diff - (seq_t)(n - 1);

    return res;
}
