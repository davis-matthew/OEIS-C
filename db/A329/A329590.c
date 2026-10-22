/*
 * A329590  Odd numbers k that cannot be expressed as k = p+q+r, with p prime
 *          and (q, r) a pair of twin primes.
 * https://oeis.org/A329590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329590: odd numbers k that cannot be expressed as
   k = p + q + r, where p is a prime and (q,r) is a twin prime pair.
   The function a(n) returns the n-th such odd number (1‑based). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* maximum value to sieve. 500 000 is enough for n ≤ 10 000. */
static const idx_t LIMIT = 500000;

/* global data, initialised once */
static bool *is_prime = NULL;          /* sieve flag array */
static idx_t *primes = NULL;           /* list of primes */
static size_t prime_cnt = 0;
static idx_t *twin_sums = NULL;        /* sums of twin prime pairs q+r */
static size_t twin_cnt = 0;
static bool *representable = NULL;     /* representable odds */

static seq_t *term_cache = NULL;       /* cached terms */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t   next_candidate = 1;      /* next odd to test */

static bool initialized = false;

/* ---------------------------------------------------------------------- */
/* initialise primes, twins and mark representable odds                  */
/* ---------------------------------------------------------------------- */
static void init_sequence(void) {
    if (initialized) return;

    /* ---------- sieve primes up to LIMIT -------------------------------- */
    is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* ---------- collect primes and twin sums --------------------------- */
    primes     = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    twin_sums  = (idx_t *)malloc((LIMIT / 2 + 1) * sizeof(idx_t));

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = i;
            if (i + 2 <= LIMIT && is_prime[i + 2])
                twin_sums[twin_cnt++] = i + (i + 2);   /* 2*i+2 */
        }
    }

    /* ---------- mark representable odds -------------------------------- */
    representable = (bool *)calloc((LIMIT + 1), sizeof(bool));

    for (size_t ti = 0; ti < twin_cnt; ++ti) {
        idx_t t = twin_sums[ti];
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = primes[pi];
            if (p == 2) continue;           /* 2 would give even sum */
            idx_t s = p + t;
            if (s > LIMIT) break;
            representable[s] = true;
        }
    }

    /* free helper arrays that are no longer needed */
    free(is_prime);
    free(primes);
    free(twin_sums);

    initialized = true;
}

/* ---------------------------------------------------------------------- */
/*  Main API: return a(n) (1‑based)                                      */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    init_sequence();

    /* return cached value if already known */
    if (n <= cache_len)
        return term_cache[n - 1];

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* skip even numbers – only odd candidates possible */
        if ((next_candidate & 1u) == 0) next_candidate++;

        if (next_candidate > LIMIT) {
            /* unlikely for requested n, but keep safe */
            return (seq_t)0;
        }

        if (!representable[next_candidate]) {
            /* append to cache */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();               /* memory exhausted */
                term_cache = tmp;
            }
            term_cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;           /* advance to next odd number */
    }

    return term_cache[n - 1];
}
