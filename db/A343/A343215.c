/*
 * A343215  Differences between consecutive numbers with unique nonzero digit
 *          in decimal representation.
 * https://oeis.org/A343215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A125289(n+1) - A125289(n) A125289: numbers whose decimal
 *            representation contains only one non‑zero digit (the digit may
 *            occur several times). The list is sorted ascending. This
 *            implementation builds the sequence lazily, generating the
 *            minimal set of numbers required for the requested n, wi...
 * Range:     u8 n <= 70, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* global cache of sorted A125289 values */
static seq_t *cached_a125289 = NULL;
static size_t cached_len   = 0;

/* function that guarantees the cache contains at least (n+1) terms
   (i.e. indices 0 … n are valid).  It grows the cache only when needed. */
static void ensure_cache_a125289(idx_t n)
{
    /* we need indices 0 … n → at least n+1 values in the array */
    const idx_t required = n + 1;

    if (cached_len >= required)
        return;                            /* cache already big enough */

    /* Determine the minimal length L such that the total number of
       distinct numbers with highest non‑zero digit at positions < L
       exceeds n.  Count of numbers with highest digit at position L-1
       is 9 * 2^(L-1).  Total up to L-1 is 9 * (2^(L-1) - 1). */
    size_t len   = 0;
    size_t total = 0;                      /* number of values already counted */

    while (total <= n) {
        ++len;                             /* consider positions up to len-1 */
        total += 9ULL * (1ULL << (len - 1));   /* new numbers added at this length */
    }

    /* Allocate new cache and free the old one if it existed */
    seq_t *new_cache = (seq_t *)malloc(total * sizeof(seq_t));
    if (!new_cache)                  /* allocation failed – nothing to do */
        return;

    /* Pre‑compute powers of 10 up to (len-1) digits */
    seq_t pow10[64];                  /* safe for len up to 63 */
    pow10[0] = (seq_t)1;
    for (idx_t i = 1; i < len; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    /* Generate numbers in strictly ascending order:
       * For each length L = 1 … len
       *   For each non‑zero digit d = 1 … 9
       *     For each subset of lower positions (0 … L-2)
       *       form the mask that has the highest bit (L-1) set
       *       and the subset bits.  Each mask gives a distinct number.
       *       The value is d * Σ 10^p over set bits. */
    size_t pos = 0;
    for (idx_t cur_len = 1; cur_len <= len; ++cur_len) {
        const size_t highest_bit = 1ULL << (cur_len - 1);
        const size_t subset_limit = 1ULL << (cur_len - 1);  /* bits 0 … L-2 */

        for (int d = 1; d <= 9; ++d) {
            const seq_t digit = (seq_t)d;
            for (size_t sub = 0; sub < subset_limit; ++sub) {
                const size_t mask = sub | highest_bit;   /* guaranteed to have
                                                           highest bit set */
                seq_t val = 0;
                for (idx_t p = 0; p < cur_len; ++p) {
                    if (mask & (1ULL << p))
                        val += digit * pow10[p];
                }
                new_cache[pos++] = val;
            }
        }
    }

    /* Replace old cache */
    free(cached_a125289);
    cached_a125289 = new_cache;
    cached_len     = total;
}

/* The required function for the harness */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* out of range – return 0 safely */

    ensure_cache_a125289(n);
    if (!cached_a125289 || (size_t)n >= cached_len)
        return (seq_t)0;                 /* safety return */

    return cached_a125289[(size_t)n] - cached_a125289[(size_t)n - 1];
}
