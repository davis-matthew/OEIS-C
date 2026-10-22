/*
 * A360930  Odd numbers which cannot be expressed as p + q*(q-1) where p and q
 *          are primes.
 * https://oeis.org/A360930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 122, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360930 – odd numbers that cannot be expressed as p + q*(q‑1)
    with p and q primes.
    The code incrementally generates the sequence by testing odd
    integers in order.  It keeps a list of primes up to √m so that
    each candidate m can be checked quickly.  The list of terms is
    cached, and successive calls to a() work in O(1) amortised time.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static seq_t *terms   = NULL;   /* cached sequence values          */
static size_t term_count = 0;   /* number of cached terms          */

static uint64_t *primes   = NULL;   /* primes ≤ current_limit           */
static size_t  prime_count = 0;     /* number of primes stored          */
static uint64_t prime_limit = 0;    /* largest prime stored             */

/* Ensure the prime list contains all primes ≤ limit.
   If the list is too short, rebuild it using the provided utility. */
static void extend_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    uint64_t *new_list = NULL;
    size_t   new_count = 0;
    build_primes_upto(limit, &new_list, &new_count);

    /* free old list if allocated */
    if (primes) free(primes);

    primes      = new_list;
    prime_count = new_count;
    prime_limit = limit;
}

/* Append a value to the cached term array, growing if necessary. */
static void cache_term(seq_t value)
{
    if (terms == NULL) {
        terms = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!terms) abort();
        term_count = 0;
    }
    if (term_count % 64 == 0) {
        size_t new_cap = (terms == NULL) ? 64 : (term_count + 64);
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
    }
    terms[term_count++] = value;
}

/* On demand compute terms until we have at least n terms. */
static void ensure_terms(size_t n)
{
    static uint64_t last_checked = 0;   /* last odd number processed   */

    /* Initialise with the first term 1 if the cache is empty. */
    if (term_count == 0) {
        cache_term((seq_t)1);           /* 1 is always non‑expressible */
        last_checked = 1;
    }

    while (term_count < n) {
        last_checked += 2;          /* next odd number */

        /* Ensure we have primes up to floor(sqrt(last_checked)). */
        uint64_t sqrt_lim = _floor_isqrt_u64(last_checked) + 1;
        extend_primes(sqrt_lim);

        bool expressible = false;

        for (size_t i = 0; i < prime_count && !expressible; ++i) {
            uint64_t q = primes[i];
            /* q*(q-1) fits in 64 bits because q ≤ sqrt(last_checked). */
            uint64_t prod = q * (q - 1);
            if (prod > last_checked) break;

            uint64_t p = last_checked - prod;
            if (p <= 1) continue;          /* p must be prime ≥ 2 */
            if (is_prime((idx_t)p)) {
                expressible = true;
                break;
            }
        }

        if (!expressible) {
            cache_term((seq_t)last_checked);
        }
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */
    size_t nn = (size_t)n;
    ensure_terms(nn);
    return terms[nn - 1];
}
