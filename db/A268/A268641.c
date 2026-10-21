/*
 * A268641  Squarefree numbers k such that k^2 + 1 and k^2 - 1 are also
 *          squarefree.
 * https://oeis.org/A268641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268641.
   Formula: a(n) = the n‑th even squarefree integer k such that
   k^2−1 and k^2+1 are also squarefree.  The function builds the
   sequence on demand by testing candidates one by one.  It keeps a
   cache of already found terms; calls with any n are answered in
   constant time after the required terms have been computed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* cached terms */
static size_t cache_sz = 0;     /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */

static uint32_t *primes = NULL; /* list of primes for factorisation */
static size_t primes_sz = 0;    /* number of primes stored */
static size_t primes_cap = 0;   /* allocated capacity for primes */

static uint64_t next_k = 2;     /* next k to test (always even) */

/* ----------  Helper: sieve to produce primes up to LIMIT  ---------- */
static void sieve_primes_up_to(uint32_t limit)
{
    /* If we already have enough primes, nothing to do. */
    if (primes && primes_sz > 0) {
        uint32_t last = primes[primes_sz - 1];
        if ((uint64_t)last * last <= limit && last >= limit) return;
    }

    /* Allocate or grow the prime array. */
    if (!primes) {
        primes_cap = limit / 10 + 10;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) abort();
    } else if (primes_sz + 1000 > primes_cap) { /* grow a bit */
        primes_cap = primes_cap * 2 + 1000;
        uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
        if (!tmp) abort();
        primes = tmp;
    }

    /* Sieve up to 'limit'.  We just run a fresh sieve each time.
       The cost is negligible for the small limits we use. */
    bool *is_comp = (bool *)calloc(limit + 1, 1);
    if (!is_comp) abort();

    primes_sz = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_sz >= primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_sz++] = i;
            if ((uint64_t)i * i > limit) continue;
            for (uint32_t j = i * i; j <= limit; j += i) is_comp[j] = true;
        }
    }
    free(is_comp);
}

/* ----------  Helper: check if a 64‑bit integer is squarefree  ---------- */
static bool squarefree_uint64(uint64_t x)
{
    if (x <= 1) return true;
    for (size_t i = 0; i < primes_sz && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return false;   /* p^2 divides the number */
        }
    }
    return true;
}

/* ----------  Generate terms until cache_sz >= n  ---------- */
static void grow_cache_to(idx_t n)
{
    /* Ensure we have enough primes to factor numbers up to (~k)^2+1.
       The maximal k we will reach is roughly proportional to n.
       Here we simply sieve up to a fixed large limit that covers all
       required k for n <= 10000.  The chosen value (1 M) is more
       than sufficient for the constraints. */
    if (!primes) sieve_primes_up_to(1000000);

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_sz < (size_t)n) {
        /* Check the current candidate k. */
        uint64_t k = next_k;
        uint64_t k2 = k * k;

        /* k must be squarefree itself. */
        if (squarefree_uint64(k) &&
            squarefree_uint64(k2 - 1) &&
            squarefree_uint64(k2 + 1)) {

            /* Store the term. */
            if (cache_sz >= cache_cap) {
                cache_cap = cache_cap * 2 + 10;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_sz++] = (seq_t)k;
        }
        next_k += 2;  /* advance to the next even integer */
    }
}

/* ----------  Public function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined index */
    if ((size_t)n > cache_sz) grow_cache_to(n);
    return cache[n - 1];
}
