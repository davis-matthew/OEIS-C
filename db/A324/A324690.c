/*
 * A324690  List of all starting positions for which all moves away from zero,
 *          if any, occur at negative positions, according to the following
 *          rules. On the k-th step (k=1,2,3,...) move a distance of k in the
 *          direction of zero. If the number landed on has been landed on
 *          before, move a distance of k away.
 * https://oeis.org/A324690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate the walk described in A324690 until 0 is hit or
   an illegal repeat occurs (a repeat when the current position is
   positive).  The function returns the n-th starting position that
   satisfies the condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum steps to guard against pathological cases (unlikely for
   the first 10000 terms). */
#define MAX_STEPS 200000

/* Test whether a given start satisfies the definition. */
static bool good(seq_t start)
{
    /* 0 is trivially good. */
    if (start == 0)
        return true;

    /* Use a signed 64‑bit type for the walk; all relevant values
       fit into this range for the required n. */
    int64_t cur = (int64_t)start;

    /* Dynamically grow the visited positions list. */
    size_t cap = 32;
    size_t len = 0;
    int64_t *vis = (int64_t *)malloc(cap * sizeof(int64_t));
    if (!vis)
        return false;
    vis[len++] = cur;

    for (size_t k = 1; k <= MAX_STEPS; ++k) {
        int64_t target;
        if (cur > 0)
            target = cur - (int64_t)k;
        else /* cur < 0 */
            target = cur + (int64_t)k;

        /* Check if target has been visited before. */
        bool repeat = false;
        for (size_t i = 0; i < len; ++i) {
            if (vis[i] == target) {
                repeat = true;
                break;
            }
        }

        if (repeat) {
            /* If the current position is positive, this is an
               illegal "away" move. */
            if (cur > 0) {
                free(vis);
                return false;
            }
            /* Away move from a negative position. */
            target = cur - (int64_t)k;
        }

        cur = target;

        /* Store the new position. */
        if (len == cap) {
            cap <<= 1;
            int64_t *tmp = (int64_t *)realloc(vis, cap * sizeof(int64_t));
            if (!tmp) {
                free(vis);
                return false;
            }
            vis = tmp;
        }
        vis[len++] = cur;

        if (cur == 0) {
            free(vis);
            return true;
        }
    }

    /* If the loop exceeded MAX_STEPS, treat the start as not good. */
    free(vis);
    return false;
}

/* Return the n-th term of sequence A324690. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len <= n) {
        /* Grow the cache to hold at least n+1 terms. */
        if (cache_cap <= n) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            while (new_cap <= (size_t)n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* fallback on allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* The next candidate start to test. */
        seq_t next_start = (cache_len == 0) ? 0 : cache[cache_len - 1] + 1;

        while (cache_len <= n) {
            if (good(next_start)) {
                cache[cache_len++] = next_start;
            }
            next_start = next_start + 1;
        }
    }

    return cache[n];
}
