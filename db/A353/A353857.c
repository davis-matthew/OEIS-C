/*
 * A353857  Numbers k such that the k-th composition in standard order has
 *          run-sum trajectory ending in a singleton.
 * https://oeis.org/A353857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353857: a(n) is the n‑th integer k such that the k‑th composition
   in standard (graded reverse‑lexicographic) order has a run‑sum
   trajectory that ends in a singleton.  This function computes the
   sequence value for any n ≥ 1.  The algorithm enumerates k = 1,2,…
   tests each k for the required property and caches all accepted k
   values so that subsequent calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Test whether the run‑sum trajectory of the k‑th composition reaches a
   singleton.  The composition is built from the bits of k as described
   in OEIS A353857. */
static bool ends_in_singleton(idx_t k)
{
    /* The representation of a composition never needs more than 64 parts. */
    unsigned int parts[64];
    unsigned int cur[64], next[64];
    unsigned int len = 0;

    /* Build the composition from the binary expansion of k.
       Bits are processed LSB first, so the resulting parts array is the
       composition in reversed order. */
    {
        idx_t temp = k;
        unsigned int prev = 0;
        unsigned int pos = 1;
        while (temp) {
            if (temp & 1ULL) {
                parts[len++] = pos - prev;
                prev = pos;
            }
            temp >>= 1;
            ++pos;
        }
        /* Reverse to obtain the standard‑order composition. */
        for (unsigned int i = 0; i < len / 2; ++i) {
            unsigned int t = parts[i];
            parts[i] = parts[len - 1 - i];
            parts[len - 1 - i] = t;
        }
    }

    if (len == 1)
        return true;               /* already a singleton */

    for (unsigned int i = 0; i < len; ++i)
        cur[i] = parts[i];
    unsigned int cur_len = len;

    /* Apply the run‑sum transformation until a singleton is reached
       or a fixed point (non‑singleton) is detected. */
    while (cur_len > 1) {
        unsigned int next_len = 0;
        unsigned int i = 0;
        while (i < cur_len) {
            unsigned int v = cur[i];
            unsigned int sum = v;
            unsigned int j = i + 1;
            while (j < cur_len && cur[j] == v) {
                sum += v;
                ++j;
            }
            next[next_len++] = sum;
            i = j;
        }

        /* If the transformed sequence equals the previous one we are
           at a fixed point that is not a singleton. */
        if (next_len == cur_len) {
            bool same = true;
            for (unsigned int t = 0; t < next_len; ++t)
                if (next[t] != cur[t]) {
                    same = false;
                    break;
                }
            if (same)
                return false;      /* fixed point, not a singleton */
        }

        if (next_len == 1)
            return true;          /* singleton reached */

        for (unsigned int t = 0; t < next_len; ++t)
            cur[t] = next[t];
        cur_len = next_len;
    }
    return true;                   /* reached singleton when len==1 */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset starts at 1 */

    /* Static cache of the first few terms that have already been found. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_cnt = 0;
    static idx_t next_k = 1;       /* next k to test */

    while (cache_cnt < n) {
        if (ends_in_singleton(next_k)) {
            /* Grow the cache if necessary. */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
