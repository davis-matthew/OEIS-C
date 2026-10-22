/*
 * A342378  List of numbers whose binary expansion is an instance of the Zimin
 *          pattern ABA.
 * https://oeis.org/A342378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342378:
   Numbers whose binary representation is an instance of the pattern ABA
   (i.e., the binary string has a non‑trivial prefix equal to a
   non‑trivial suffix).  The nth term (n ≥ 1) is returned.  The
   implementation keeps a static cache and extends it only until the
   requested index is reached.  Time complexity is O(n·log n), easily
   under a second for n up to 10 000. */

#include <stdlib.h>          // for malloc, realloc
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"   // bitlen_seq_t(seq_t x)

/* Return true iff x has a non‑trivial border of length a (1 ≤ a < L/2). */
static bool hasBorder(seq_t x) {
    idx_t L = bitlen_seq_t(x);          // number of bits in x
    if (L < 3) return false;           // need at least ABA
    idx_t maxA = (L - 1) / 2;          // a < L/2 ensures B non‑empty
    unsigned int bits = sizeof(seq_t) * 8;  // width of seq_t
    for (idx_t a = 1; a <= maxA; ++a) {
        /* extract high a bits: prefix */
        seq_t high = x >> (L - a);

        /* extract low a bits: suffix */
        seq_t low;
        if (a == bits) {               // avoid shift by width
            low = (seq_t)-1;
        } else {
            seq_t mask = ((seq_t)1 << a) - 1;
            low = x & mask;
        }

        if (high == low) return true;
    }
    return false;
}

/* Static cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;
static seq_t next_candidate = 5;        /* first term in OEIS */

/* Ensure the cache can hold at least `needed` elements */
static void ensure_cache_cap(idx_t needed) {
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap) {
        /* Allocation failed – abort */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms until we have at least `n` terms in the cache */
static void generate_until(idx_t n) {
    while (cache_size < n) {
        if (hasBorder(next_candidate)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Exported function: a(n) for n ≥ 1 */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n == 0, return 0 as placeholder */
    generate_until(n);
    return cache[n-1];
}
