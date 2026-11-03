/*
 * A356956  Numbers k such that the k-th composition in standard order is a
 *          gapless interval (in increasing order).
 * https://oeis.org/A356956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonnegative integer k such that the k-th
 *            composition in standard order is an increasing consecutive
 *            interval (a 0-length composition earns the value 0)
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether the composition produced by k in standard order is a
   gapless interval (consecutive increasing parts). */
static inline bool is_gapless_interval(idx_t k)
{
    if (k == 0) return true;          /* empty composition */

    /* positions of 1 bits, counting from 1, plus the leading 0 */
    int pos[70];
    int posCnt = 1;                    /* pos[0] = 0 already */
    pos[0] = 0;

    for (int i = 1; (k >> (i - 1)) != 0; ++i) {
        if (k & (1ULL << (i - 1))) {
            pos[posCnt++] = i;
        }
    }

    int L = posCnt - 1;                /* number of composition parts */
    if (L == 0) return true;

    int diff[70];
    for (int i = 1; i < posCnt; ++i) {
        diff[i - 1] = pos[i] - pos[i - 1];
    }

    /* reverse order of differences and check for consecutive 1 increments */
    int prev = diff[L - 1];
    for (int t = L - 2; t >= 0; --t) {
        int cur = diff[t];
        if (cur != prev + 1) return false;
        prev = cur;
    }
    return true;
}

/* Return the n-th element of A356956 (offset 1, a(1)=0).  Uses a
   simple incremental cache so that calls in arbitrary order are fast. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 */

    /* static cache of already-found terms */
    static idx_t *cache   = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_k = 0;       /* next candidate to test */

    if (cache == NULL) {
        cache_cap = 128;
        cache = (idx_t *)malloc(sizeof(idx_t) * cache_cap);
    }

    while (cache_len < n) {
        while (!is_gapless_interval(next_k)) ++next_k;

        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (idx_t *)realloc(cache, sizeof(idx_t) * cache_cap);
        }
        cache[cache_len++] = next_k;
        ++next_k;
    }

    return (seq_t)cache[n - 1];
}
