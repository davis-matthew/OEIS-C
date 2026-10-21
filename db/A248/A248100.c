/*
 * A248100  Number of ordered trees with n edges such that non-leaf vertices
 *          at even levels have outdegree 1 and those at odd levels have
 *          outdegree 2.
 * https://oeis.org/A248100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(0)=1, a(1)=1, a(2)=0, and for n≥3 a(n)=∑_{k=0}^{n-3}
 *            a(k)*a(n-3−k) (Catalan‑like convolution shifted by 3)
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if ((size_t)needed <= cache_len) return;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
    if (!new_cache) abort();

    for (idx_t i = cache_len; i < needed; ++i) {
        if (i == 0) {
            new_cache[0] = (seq_t)1;
        } else if (i == 1) {
            new_cache[1] = (seq_t)1;
        } else if (i == 2) {
            new_cache[2] = (seq_t)0;
        } else {
            seq_t sum = (seq_t)0;
            idx_t maxk = i - 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                sum += new_cache[k] * new_cache[maxk - k];
            }
            new_cache[i] = sum;
        }
    }

    cache = new_cache;
    cache_len = (size_t)needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
