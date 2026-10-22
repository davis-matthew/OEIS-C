/*
 * A381823  Odd cubefree numbers that are not squarefree.
 * https://oeis.org/A381823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 4590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd cubefree numbers that are not squarefree
   (OEIS A381823).  The nth term is the nth odd integer whose prime
   factorisation contains only odd primes, no exponent ≥ 3, and at least
   one exponent equal to 2. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* cached terms of the sequence */
static seq_t *seq_cache   = NULL;  /* array of terms */
static size_t cache_len   = 0;     /* number of terms stored      */
static size_t cache_cap   = 0;     /* allocated capacity          */
static uint64_t next_cand = 1;     /* next odd integer to test    */

/* grow the cache to at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    /* cast is required for C++ */
    seq_t *p = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    seq_cache = p;
    cache_cap = new_cap;
}

/* test whether an odd integer cand satisfies the conditions
   (odd , only odd primes, no exponent ≥ 3, at least one exponent = 2) */
static bool valid_candidate(uint64_t cand)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(cand, pr, ex, &cnt);

    bool has_square = false;
    for (size_t i = 0; i < cnt; ++i) {
        /* even prime factor forbids odd-only condition */
        if (pr[i] == 2) return false;
        if (ex[i] > 2) return false;          /* not cubefree */
        if (ex[i] == 2) has_square = true;    /* at least one square */
    }
    return has_square;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behavior below offset is irrelevant */

    /* generate terms until cache contains at least n elements */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_cand;
        if (valid_candidate(cand)) {
            grow_cache(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)cand;
        }
        next_cand = cand + 2;              /* skip even numbers */
    }
    return seq_cache[(size_t)n - 1];
}
