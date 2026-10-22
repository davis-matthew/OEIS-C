/*
 * A351596  Numbers k such that the k-th composition in standard order has all
 *          distinct run-lengths.
 * https://oeis.org/A351596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that the k-th composition in
 *            standard order (graded reverse‑lexicographic, A066099) has all
 *            distinct run‑lengths. The sequence is indexed with offset 1
 *            (a(1)=0)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool qualifies(idx_t k)
{
    /* collect positions of 1's in reversed binary of k (1‑based) */
    int pos[65];
    int posCnt = 0;
    int bitPos = 1;
    idx_t temp = k;
    while (temp) {
        if (temp & 1ULL)
            pos[posCnt++] = bitPos;
        temp >>= 1;
        bitPos++;
    }

    if (posCnt == 0) /* k == 0 → empty composition → trivially true */
        return true;

    /* compute differences (first differences after prepending 0) */
    int diff[65];
    int prev = 0;
    for (int i = 0; i < posCnt; ++i) {
        diff[i] = pos[i] - prev;
        prev = pos[i];
    }

    /* reverse the differences to obtain the composition */
    int comp[65];
    for (int i = 0; i < posCnt; ++i)
        comp[i] = diff[posCnt - 1 - i];

    /* test distinctness of run‑lengths */
    bool used[65] = {0};
    int runLen = 1;
    for (int i = 1; i < posCnt; ++i) {
        if (comp[i] == comp[i - 1]) {
            ++runLen;
        } else {
            if (used[runLen])
                return false;
            used[runLen] = true;
            runLen = 1;
        }
    }
    if (used[runLen])
        return false;
    used[runLen] = true;
    return true;
}

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0; /* next candidate integer to test */

    if (n == 0)
        return (seq_t)0; /* undefined by sequence definition, but safe */

    /* initialize cache lazily */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        if (qualifies(next_k)) {
            if (cache_len == cache_cap) {          /* grow cache */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
