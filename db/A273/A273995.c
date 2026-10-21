/*
 * A273995  Even numbers with a unique representation as the difference of two
 *          primes, each of which is a member of a pair of twin primes, and
 *          one of which is smaller than the even number under consideration.
 * https://oeis.org/A273995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smallest even number that has exactly one
 *            representation as q = p – q, where p and q are primes, each of
 *            which is member of a twin prime pair, and q < p – q
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   make_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds all such numbers up to the largest even
   required for the requested index, storing them in a static array
   for memoisation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/make_is_prime.h"

/* ------------------------------------------------------------------ */
/* Static data for the sequence                                       */
/* ------------------------------------------------------------------ */
static idx_t   current_limit   = 0;   /* the sieve limit used in the last call to compute_sequence */
static seq_t * terms            = NULL;  /* array of sequence terms */
static size_t terms_len        = 0;       /* number of terms stored */
static size_t terms_cap        = 0;       /* capacity of terms[] */

/* ------------------------------------------------------------------ */
/* Ensure that terms[] has at least 'needed' capacity                  */
/* ------------------------------------------------------------------ */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Allocate a bool array large enough for 0 .. limit, all set true    */

/* ------------------------------------------------------------------ */
/* Compute all terms up to the even number 'limit' (inclusive)        */
/* ------------------------------------------------------------------ */
static void compute_sequence(idx_t limit)
{
    /* free previous terms */
    free(terms);
    terms      = NULL;
    terms_len  = 0;
    terms_cap  = 0;

    /* Sieve up to 2*limit + 10, because a difference d <= limit
       may involve a prime as large as limit + d <= 2*limit.            */
    idx_t prime_lim = limit * 2ULL + 10ULL;
    bool *is_prime  = make_is_prime(prime_lim);

    /* First pass: collect all primes that belong to a twin pair.
       We keep them in increasing order.                                 */
    uint32_t *twin = NULL;
    size_t twin_cap = 1024, twin_len = 0;
    twin = (uint32_t *)malloc(twin_cap * sizeof(uint32_t));
    if (!twin) abort();

    for (idx_t n = 3; n <= prime_lim; n += 2) {   /* skip even numbers >2 */
        if (!is_prime[n]) continue;
        bool is_twin = false;
        if (n > 2 && is_prime[n - 2]) is_twin = true;
        if (n + 2 <= prime_lim && is_prime[n + 2]) is_twin = true;
        if (is_twin) {
            if (twin_len >= twin_cap) {
                twin_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(twin, twin_cap * sizeof(uint32_t));
                if (!tmp) abort();
                twin = tmp;
            }
            twin[twin_len++] = (uint32_t)n;
        }
    }

    /* Array to count representations of each even difference <= limit. */
    int *cnt = (int *)calloc((size_t)limit + 1, sizeof(int));
    if (!cnt) abort();

    /* Count each pair (q,p) with q<p, both in twin list, p-q <= limit,
       and q < (p-q).  Because the twin list is sorted, we break inner
       loop early when the difference exceeds limit.                    */
    for (size_t i = 0; i < twin_len; ++i) {
        uint32_t q = twin[i];
        for (size_t j = i + 1; j < twin_len; ++j) {
            uint32_t p = twin[j];
            uint32_t d = p - q;
            if (d > (uint32_t)limit) break;
            if (q < d) cnt[d]++;
        }
    }

    /* Collect the even differences with exactly one representation.    */
    for (uint32_t d = 4; d <= (uint32_t)limit; d += 2) {
        if (cnt[d] == 1) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)d;
        }
    }

    /* Clean up */
    free(is_prime);
    free(cnt);
    free(twin);
    current_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Grow the sieve and recompute until we have at least 'needed' terms. */
/* ------------------------------------------------------------------ */
static void ensure_terms(int needed)
{
    if ((size_t)needed <= terms_len) return;
    /* Starting limit: if none yet, start reasonably small */
    idx_t limit = current_limit ? current_limit : 5000ULL;
    /* Grow until we have enough terms */
    while ((size_t)needed > terms_len) {
        compute_sequence(limit);
        /* Double the limit for next attempt, but cap to avoid runaway */
        if (limit > 5000000ULL) break;   /* defensive; terms should be found earlier */
        limit <<= 1;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour for index < 1 is unspecified */
    ensure_terms((int)n);
    if ((size_t)n > terms_len) return 0;  /* undefined beyond computed range */
    return terms[(size_t)n - 1];
}
