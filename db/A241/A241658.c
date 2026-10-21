/*
 * A241658  Smallest semiprime, sp, such that n - sp is a semiprime, or a(n)=0
 *          if there is no such sp.
 * https://oeis.org/A241658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest semiprime sp such that n‑sp is a semiprime, 0 if
 *            no such semiprime exists. Indexing starts at 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   build_spf, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Utility includes.  The build system supplies these headers. */
#include "../utils/build_spf.h"     /* void build_spf(idx_t limit, idx_t **spf_out); */
#include "../utils/is_prime_small.h"/* bool is_prime_small(seq_t n); */

/* Static caches for semiprimes and smallest‑prime factors. */
static idx_t *spf_cache = NULL;    /* smallest prime factor array */
static idx_t spf_cache_size = 0;   /* size of the spf_cache (maximum index cached) */

static uint8_t *semiprime_cache = NULL; /* 1 if index is a semiprime, else 0 */
static idx_t semiprime_cache_size = 0;   /* maximum index cached */

/* Ensure caches contain information up to at least limit. */
static void ensure_cache(idx_t limit)
{
    if (limit <= spf_cache_size) return;  /* already cached */

    /* Free previous caches if any. */
    if (spf_cache) free(spf_cache);
    if (semiprime_cache) free(semiprime_cache);

    /* Build smallest‑prime‑factor table up to limit. */
    build_spf(limit, &spf_cache);
    spf_cache_size = limit;

    /* Allocate semiprime boolean array. */
    semiprime_cache = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    semiprime_cache_size = limit;
    for (idx_t i = 0; i <= limit; ++i) semiprime_cache[i] = 0;

    /* Mark semiprimes using the SPF array. */
    for (idx_t n = 4; n <= limit; ++n) {
        idx_t p = spf_cache[n];
        if (p == n) continue;                /* prime -> not semiprime */
        idx_t q = n / p;
        if (q > 1 && is_prime_small((seq_t)q))
            semiprime_cache[n] = 1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* safety for out‑of‑bounds calls */

    /* Smallest semiprime is 4; need n‑sp >= 4 => n >= 8. */
    if (n < 8) return 0;

    ensure_cache(n);

    /* Search for the smallest semiprime sp with n‑sp also a semiprime. */
    for (idx_t sp = 4; sp <= n - 4; ++sp) {
        if (semiprime_cache[sp] && semiprime_cache[n - sp])
            return (seq_t)sp;
    }
    return 0;
}
