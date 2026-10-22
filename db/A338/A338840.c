/*
 * A338840  No even digit is present in a(n) + a(n+1).
 * https://oeis.org/A338840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   no simple formula – lexicographically earliest permutation * of
 *            the nonnegative integers such that for all n, * a(n)+a(n+1) has
 *            no even decimal digit. * The function greedily builds the
 *            sequence, storing the terms in a * cache and the used numbers in
 *            a dynamic bit‑set
 * Range:     u8 n <= 211, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   no_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/no_even_digits.h"

static seq_t   *seq_cache = NULL;   /* cache of computed terms          */
static size_t  seq_len   = 0;        /* number of stored terms           */
static size_t  seq_cap   = 0;        /* allocated capacity for seq_cache */

static uint64_t *used_bits = NULL;   /* bit‑set of already used integers */
static size_t   used_cap   = 0;       /* capacity of the bit‑set          */

/* ensure seq_cache has capacity at least 'need' */
static inline void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) exit(1);
    seq_cap = new_cap;
}

/* ensure used_bits covers bit index 'need' */
static inline void ensure_used_capacity(size_t need)
{
    size_t need_words = (need >> 6) + 1;
    if (used_cap >= need_words) return;
    size_t new_cap = used_cap ? used_cap << 1 : 4;
    while (new_cap < need_words) new_cap <<= 1;
    used_bits = (uint64_t *)realloc(used_bits, new_cap * sizeof(uint64_t));
    if (!used_bits) exit(1);
    for (size_t i = used_cap; i < new_cap; ++i) used_bits[i] = 0;
    used_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n<1, return 0 */

    /* initialise on first call */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq_cache[0] = (seq_t)0;
        seq_len = 1;
        ensure_used_capacity(0);
        used_bits[0] |= 1ULL;          /* mark 0 as used */
    }

    if (n <= seq_len) return seq_cache[n-1];

    /* extend cache up to requested index */
    for (idx_t i = seq_len; i < n; ++i) {
        seq_t prev = seq_cache[i-1];
        seq_t cand = 0;

        /* search for the smallest unused integer that keeps sum free of even digits */
        for (;;) {
            size_t cand_u = (size_t)cand; /* cand fits in size_t for our purposes */
            ensure_used_capacity(cand_u);
            if (used_bits[cand_u >> 6] & (1ULL << (cand_u & 63))) { /* already used */
                ++cand;
                continue;
            }
            seq_t sum = prev + cand;
            if (no_even_digits(sum)) {
                /* Mark as used and store */
                used_bits[cand_u >> 6] |= (1ULL << (cand_u & 63));
                ensure_seq_capacity(i+1);
                seq_cache[i] = cand;
                break;
            }
            ++cand;
        }
    }
    seq_len = n;
    return seq_cache[n-1];
}
