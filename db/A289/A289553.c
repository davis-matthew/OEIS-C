/*
 * A289553  Numbers that are not the product of two distinct noncomposite
 *          numbers (A008578).
 * https://oeis.org/A289553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that τ(k) ≠ 2·ω(k). Implemented by sieving. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Static cache for computed sequence terms */
static seq_t *terms      = NULL;  /* array of sequence values   */
static size_t terms_len  = 0;     /* number of terms already stored */
static size_t terms_cap  = 0;     /* capacity of 'terms' array    */

/* The largest integer that has been processed so far   */
static idx_t processed_limit = 0;

/* Extend the cached terms up to 'new_limit'.  The function first
   builds the array of smallest prime factors up to 'new_limit'
   and then scans the integers from (processed_limit+1) to
   'new_limit', computing τ(i) and ω(i) for each and storing the
   values that satisfy τ(i) ≠ 2·ω(i). */
static void extend_cache(idx_t new_limit)
{
    /* Build SPF array.  The helper returns an array indexed
       0..new_limit with spf[i] = the smallest prime factor of i
       (and spf[i] = i for primes). */
    idx_t *spf = NULL;
    build_spf(new_limit, &spf);
    if (spf == NULL) return;   /* fail quietly – caller will retry with larger limit */

    for (idx_t i = processed_limit + 1; i <= new_limit; ++i) {
        /* Factor i using the SPF array.  Compute τ(i) (divisor count)
           and ω(i) (number of distinct prime factors). */
        idx_t n = i;
        unsigned long long tau = 1;
        unsigned long long omega = 0;

        while (n > 1) {
            idx_t p = spf[n];
            unsigned long long cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
            }
            tau *= (cnt + 1);
            ++omega;
        }

        if (tau != 2 * omega) {
            /* Ensure we have capacity in the terms array. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (tmp == NULL) { free(spf); return; }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)i;
        }
    }

    /* Clean up temporary SPF array. */
    free(spf);
    processed_limit = new_limit;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined index – harmless */

    /* Generate more terms until we have at least 'n' terms cached. */
    while (terms_len < n) {
        /* Choose a new limit that is larger than the previous one and
           sufficient to likely yield the required number of terms. */
        idx_t next_limit = processed_limit ? processed_limit * 2 : 4;
        /* If our estimate is too low, increase more aggressively. */
        if (next_limit < processed_limit + (n - terms_len) * 3)
            next_limit = processed_limit + (n - terms_len) * 3;

        extend_cache(next_limit);
    }

    return terms[n - 1];
}
