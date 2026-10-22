/*
 * A341935  a(0) = 0; for n > 0, a(n) is the smallest positive integer not yet
 *          in the sequence that can be created by adding 1 or -1, for digits
 *          > 0, to every digit in a(n-1).
 * https://oeis.org/A341935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the smallest positive integer not yet in the sequence
 *            that can be created by adding 1 or -1 (for digits > 0) to every
 *            digit of a(n-1). The sequence is finite; after a(28)=30 it
 *            terminates
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_str.h"

/* Helper to grow a dynamically allocated array. */
static void grow_cache(seq_t **ptr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 1 : *cap;
        while (newcap < needed) newcap <<= 1;
        seq_t *newp = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
        if (!newp) exit(1);            /* out of memory */
        *ptr = newp;
        *cap = newcap;
    }
}

/* Recursive DFS that builds all digit‑transform candidates. */
static void dfs_generate(const char *orig, int orig_len, int idx,
                         char *buf, int bufpos, seq_t *best,
                         const seq_t *cache, idx_t cache_len)
{
    if (idx == orig_len) {
        seq_t val = 0;
        for (int i = 0; i < bufpos; ++i)
            val = val * 10 + (seq_t)(buf[i] - '0');

        if (val >= 1) {
            bool seen = false;
            for (idx_t j = 0; j < cache_len; ++j)
                if (cache[j] == val) { seen = true; break; }
            if (!seen && (*best == 0 || val < *best))
                *best = val;
        }
        return;
    }

    char c = orig[idx];
    int d = c - '0';

    if (d == 0) {
        buf[bufpos] = '1';
        dfs_generate(orig, orig_len, idx + 1, buf, bufpos + 1, best, cache, cache_len);
    } else {
        /* d - 1 */
        buf[bufpos] = '0' + (d - 1);
        dfs_generate(orig, orig_len, idx + 1, buf, bufpos + 1, best, cache, cache_len);

        /* d + 1 */
        int v2 = d + 1;
        if (v2 < 10) {
            buf[bufpos] = '0' + v2;
            dfs_generate(orig, orig_len, idx + 1, buf, bufpos + 1, best, cache, cache_len);
        } else {                  /* v2 == 10 */
            buf[bufpos]     = '1';
            buf[bufpos + 1] = '0';
            dfs_generate(orig, orig_len, idx + 1, buf, bufpos + 2, best, cache, cache_len);
        }
    }
}

/* Generate the next sequence element given the previous one. */
static seq_t generate_next(const seq_t prev, const seq_t *cache,
                           idx_t cache_len, bool *found)
{
    char orig[128];
    size_t orig_len;
    seq_to_str(prev, orig, &orig_len);
    orig[orig_len] = '\0';

    char buf[256];
    seq_t best = 0;
    dfs_generate(orig, (int)orig_len, 0, buf, 0, &best, cache, cache_len);
    *found = (best != 0);
    return best;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static bool finished = false;

    /* initialisation */
    if (cache_len == 0) {
        grow_cache(&cache, &cache_cap, 1);
        cache[0] = 0;
        cache_len = 1;
    }

    if (n < cache_len)
        return cache[n];

    if (finished)
        return cache[cache_len - 1];

    while (cache_len <= n && !finished) {
        bool found;
        seq_t next = generate_next(cache[cache_len - 1], cache, cache_len, &found);
        if (!found) {
            finished = true;
            break;
        }
        grow_cache(&cache, &cache_cap, cache_len + 1);
        cache[cache_len] = next;
        cache_len++;
    }

    if (n < cache_len)
        return cache[n];
    return cache[cache_len - 1];
}
