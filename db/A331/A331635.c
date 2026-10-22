/*
 * A331635  Numbers m such that -m^2 == m (mod sigma(m)) where sigma =
 *          A000203.
 * https://oeis.org/A331635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   m such that -m^2 == m (mod sigma(m)) Equivalent: m*(m+1) is a
 *            multiple of sigma(m), where sigma is A000203
 * Range:     u8 none, u16 n <= 2467, u32 n <= 6028, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper from the supplied library */
#include "../utils/sigma_of.h"

/* --------------------------------------------------------------------
 * Internal state: we keep all found terms as 64‑bit integers.  The
 * sequence values are returned as seq_t by casting; this works because
 * the sequence values are small enough for all requested indices.  All
 * arithmetic inside the search is done in seq_t to avoid overflows in
 * the modular congruence test.
 * -------------------------------------------------------------------- */

struct seq_state {
    uint64_t *terms;   /* stored terms (fitted in 64 bits)           */
    size_t   cap;      /* allocated capacity                        */
    idx_t    len;      /* number of stored terms                    */
    idx_t    processed;/* largest m processed so far                */
    uint64_t limit;    /* current upper bound for the sieve         */
};

static struct seq_state state = {NULL, 0, 0, 0, 0};

/* Grow the terms array to accommodate at least `needed` elements. */
static void grow_terms(size_t needed)
{
    if (state.cap >= needed) return;
    size_t new_cap = state.cap ? state.cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    if (!tmp) abort();              /* out of memory */
    if (state.terms) {
        for (size_t i = 0; i < state.len; ++i) tmp[i] = state.terms[i];
        free(state.terms);
    }
    state.terms = tmp;
    state.cap   = new_cap;
}

/* Ensure that the first `n` terms have been generated. */
static void ensure_terms(idx_t n)
{
    if (state.len >= n) return;

    /* Choose a sieve limit that is likely to contain at least n terms. */
    uint64_t new_limit = state.limit;
    if (new_limit < 1000ULL) new_limit = 1000ULL;
    if (new_limit < (uint64_t)n * 20ULL) new_limit = (uint64_t)n * 20ULL;

    /* No need to rebuild sieve if limit did not change. */
    state.limit = new_limit;

    /* Generate terms for m = processed + 1 … limit. */
    for (idx_t m = state.processed + 1; m <= state.limit; ++m) {
        seq_t sigma = sigma_of((idx_t)m);          /* σ(m) */
        seq_t prod  = ((seq_t)m * ((seq_t)m + 1)) % sigma;
        if (prod == 0) {
            grow_terms(state.len + 1);
            state.terms[state.len++] = (uint64_t)m;
        }
    }
    state.processed = state.limit;

    /* If still not enough terms, double the limit and recurse. */
    if (state.len < n) {
        state.limit *= 2;
        ensure_terms(n);
    }
}

/* --------------------------------------------------------------------
 * Public function required by the harness.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, harmless */
    ensure_terms(n);
    return (seq_t)state.terms[n - 1];
}
