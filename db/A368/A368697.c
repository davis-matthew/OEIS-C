/*
 * A368697  Numbers that are divisible by the squares of two distinct primes
 *          and whose arithmetic derivative (A003415) is a squarefree number
 *          of the form 4k+2.
 * https://oeis.org/A368697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bitlen_seq_t, factor_u64, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368697: numbers divisible by squares of two distinct primes
   whose arithmetic derivative is a square‑free integer of the form 4k+2. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/seq_alloc_new_array.h"

/* test if a 64‑bit integer satisfies the defining properties.  The
   arithmetic derivative of n is Σ e_i * n/p_i, where n = Π p_i^{e_i}. */
static bool satisfies_uint64(uint64_t n) {
    /* a(1) = 11025, so anything smaller cannot be a term */
    if (n < 11025u) return false;

    /* factor n */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t pf_cnt;
    factor_u64(n, primes, exps, &pf_cnt);

    /* need at least two distinct prime squares */
    int sq = 0;
    for (size_t i = 0; i < pf_cnt; ++i)
        if (exps[i] >= 2) ++sq;
    if (sq < 2) return false;

    /* arithmetic derivative */
    seq_t der = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t t = (seq_t)(n / primes[i]) * (seq_t)exps[i];
        der += t;
    }

    /* derivative must be 2 mod 4 */
    if ((der & 3) != 2) return false;

    /* derivative must be squarefree */
    if (bitlen_seq_t(der) > 63) return false;          /* too large to factor */
    uint64_t d = (uint64_t)der;
    factor_u64(d, primes, exps, &pf_cnt);
    for (size_t i = 0; i < pf_cnt; ++i)
        if (exps[i] > 1) return false;

    return true;
}

/* resize cache to hold at least 'need' elements */
static void ensure_capacity(size_t need, size_t *cap, seq_t **ptr) {
    if (*cap >= need) return;
    size_t newcap = (*cap ? (*cap << 1) : 16);
    while (newcap < need) newcap <<= 1;
    seq_t *newp = seq_alloc_new_array(newcap);
    if (*ptr) {
        memcpy(newp, *ptr, (*cap) * sizeof(seq_t));
        free(*ptr);
    }
    *ptr = newp;
    *cap = newcap;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;         /* undefined for n < 1 */

    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 11025u;   /* first possible term */

    /* grow cache until it contains n terms */
    while (cache_len < (size_t)n) {
        /* find the next integer that satisfies the conditions */
        while (!satisfies_uint64(next_candidate))
            ++next_candidate;

        /* store the term */
        ensure_capacity(cache_len + 1, &cache_cap, &cache);
        cache[cache_len] = (seq_t)next_candidate;
        ++cache_len;
        ++next_candidate;
    }

    return cache[n - 1];
}
