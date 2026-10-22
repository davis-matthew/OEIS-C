/*
 * A357759  Numbers k such that in the binary expansion of k, the Hamming
 *          weight of each block differs by at most 2 from every other block
 *          of the same length.
 * https://oeis.org/A357759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 234, u16 n <= 7658, u32 n <= 7658, u64 n <= 7658,
 *            u128 n <= 7658, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A357759:
   a(n) = the n‑th non‑negative integer k such that in the binary
   expansion of k, for every block length L, the Hamming weights of
   all contiguous blocks of length L differ by at most 2.
   The implementation searches successive integers, tests each one
   with the described condition, and caches the results so that any
   call to a(idx_t n) is answered in O(1) after the initial scan. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return the number of bits needed to represent x (0 → 0). */
static unsigned bitlen_seq_t_custom(seq_t x)
{
    unsigned len = 0;
    while (x > (seq_t)0) {
        len++;
        x >>= 1;
    }
    return len;
}

/* Return true if k satisfies the block‐weight condition. */
static bool good(seq_t k)
{
    if (k == (seq_t)0)        /* 0 is always included */
        return true;

    unsigned blen = bitlen_seq_t_custom(k);
    for (unsigned L = 1; L <= blen; ++L) {
        unsigned minw = (unsigned)-1;   /* max unsigned */
        unsigned maxw = 0;
        unsigned windows = blen - L + 1;
        for (unsigned i = 0; i < windows; ++i) {
            /* Count ones in the L‑bit window starting at position i. */
            unsigned w = 0;
            unsigned start_bit = blen - 1 - i;          /* msb position of the start */
            for (unsigned j = 0; j < L; ++j) {
                unsigned shift = start_bit - j;
                if ( ((k >> shift) & (seq_t)1) != (seq_t)0 )
                    ++w;
            }
            if (w < minw) minw = w;
            if (w > maxw) maxw = w;
            if (maxw - minw > 2)
                return false;
        }
    }
    return true;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)              /* out of range requests are ignored */
        return (seq_t)0;

    /* Static cache of found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t  last_checked = 0;   /* last integer examined */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure, return 0 */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)last_checked;
        if (good(candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failure, return 0 */
                cache = tmp;
            }
            cache[cache_len++] = candidate;
        }
        ++last_checked;
    }

    /* Return the (n‑1)‑th index (since a(1) is the first term). */
    return cache[n - 1];
}
