/*
 * A333227  Numbers k such that the k-th composition in standard order is
 *          pairwise coprime, where a singleton is not coprime unless it is
 *          (1).
 * https://oeis.org/A333227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A333227: returns the n‑th integer k such that the k‑th composition in
   standard order is pairwise coprime.  The routine iterates over compositions
   in order using binary decomposition of k and selects those whose parts
   are pairwise coprime (singletons are allowed only when the part is 1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_k = 1;

static bool is_pairwise_coprime(idx_t k)
{
    idx_t tmp = k;
    idx_t S = 0;
    while (tmp) {
        S++;
        tmp >>= 1;
    }
    /* S ≥ 1 for k ≥ 1 */
    idx_t start = 1ULL << (S - 1);
    idx_t r = k - start;

    /* Build parts of the composition */
    idx_t parts[64];
    int len = 0;
    idx_t cur = 1;
    for (idx_t i = 0; i < S - 1; ++i) {
        idx_t bitpos = S - 2 - i;
        idx_t bit = (r >> bitpos) & 1ULL;
        if (bit) {
            parts[len++] = cur;
            cur = 1;
        } else {
            cur++;
        }
    }
    parts[len++] = cur;

    if (len == 1) {
        return parts[0] == 1;
    }

    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (gcd((seq_t)parts[i], (seq_t)parts[j]) != (seq_t)1) {
                return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, guard against misuse */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_size < (size_t)n) {
        if (is_pairwise_coprime(next_k)) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
