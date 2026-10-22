/*
 * A342051  Numbers k which have an even number of trailing zeros in their
 *          primorial base representation A049345(k).
 * https://oeis.org/A342051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342051
   Numbers k whose primorial‑base representation has an even number
   of trailing zeros.  For k, let m be the largest integer such that
   k is divisible by the product of the first m primes.  Then k is a
   term iff m is even.  This routine returns the n‑th such k
   (n≥1).  The implementation scans successive integers, tests the
   condition, and caches the results to be independent of the call
   order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cache of already computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of terms cached          */
static size_t cache_cap = 0;      /* current capacity of the cache   */

/* Next integer to be tested for membership in the sequence */
static idx_t next_candidate = 1;

/* First few primes – enough for the primorials needed for all
   terms up to the 10 000th entry of the sequence. */
static const uint64_t small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL,
    79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL,
    107ULL, 109ULL, 113ULL
};
static const size_t small_primes_cnt = sizeof small_primes / sizeof small_primes[0];

/* Helper: grow the cache if necessary – double the capacity. */
static void grow_cache(void) {
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Allocation failed – abort. */
        exit(EXIT_FAILURE);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Test whether the integer k has an even number of trailing zeros
   in its primorial‑base representation. */
static bool even_primorial_zeros(seq_t k) {
    seq_t prod = 1;
    size_t trailing = 0;

    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint64_t p = small_primes[i];

        /* Prevent overflow for fixed‑width seq_t (e.g. uint64_t). */
        if (prod > k / (seq_t)p) {
            break;
        }
        prod *= (seq_t)p;

        if (k % prod == 0) {
            ++trailing;           /* one more trailing zero found   */
        } else {
            break;                 /* first failure stops the run   */
        }
    }
    return ((trailing & 1U) == 0);  /* true if trailing # is even */
}

/* Return the n‑th term of A342051.  Behaviour for n<=0 is undefined
   but we return 0 as a safety guard. */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Generate terms until we have at least n cached. */
    while (cache_len < (size_t)n) {
        if (even_primorial_zeros((seq_t)next_candidate)) {
            if (cache_len + 1 > cache_cap) {
                grow_cache();
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
