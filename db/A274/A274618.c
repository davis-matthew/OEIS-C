/*
 * A274618  Sprague-Grundy values for the game of duplicate Double Kayles
 *          (octal game .07DDDD7).
 * https://oeis.org/A274618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sprague–Grundy value for the octal game .07DDDD7 *
 *            (duplicate Double Kayles). The function computes the Grundy
 *            numbers * by dynamic programming: for each heap size n, it takes
 *            the mex of all * xor‑values of reachable positions according to
 *            the rule digits: * k = 1 : 0 (no move), k = 2…7 :...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* maximum Grundy value for which we use a marker array during mex
 * computation; larger values are handled by ignoring the mark (the
 * mex will be greater than this limit). */
static const unsigned long MAX_MEX_MARK = 65536UL;

/* marker array for computing mex; values store the “tag” of the current
 * computation.  The array is allocated lazily. */
static unsigned int *seen = NULL;
static unsigned int seen_len = 0;
static unsigned int seen_tag = 1;   /* tag counter */

/* cache of already computed Grundy values (dynamic sizing). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* octal digits for move lengths 1…7.  The game allows no move of
 * length 1, and for lengths 2–7 all splits are allowed. */
static const uint32_t octal_digits[8] = {
    0,      /* unused index 0 */
    0,      /* k = 1 : no move */
    7,      /* k = 2 : all splits allowed (binary 111) */
    7,      /* k = 3 */
    7,      /* k = 4 */
    7,      /* k = 5 */
    7,      /* k = 6 */
    7       /* k = 7 */
};

/* ensure the seen array is large enough and clear it on first use.
 * The array is reused for each mex computation; it is cleared only
 * when we reset the tag counter after an overflow. */
static void ensure_seen_array(void)
{
    if (seen_len < MAX_MEX_MARK) {
        free(seen);
        seen = (unsigned int*)malloc(MAX_MEX_MARK * sizeof(unsigned int));
        seen_len = MAX_MEX_MARK;
    }
    /* clear on first run */
    if (seen_tag == 1) {
        for (size_t i = 0; i < MAX_MEX_MARK; ++i) seen[i] = 0;
    }
}

/* enlarge the cache so that it can hold indices up to max_n.
 * We grow it at most once for each request to keep the number of
 * reallocations minimal. */
static void grow_cache(idx_t max_n)
{
    if (max_n < cache_size) return;
    idx_t new_size = max_n + 1;                     /* include max_n */
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);             /* allocation failure */
    cache = new_cache;
    cache_size = new_size;
}

/* compute Grundy value a(n). */
seq_t a(idx_t n)
{
    if (n < cache_size) return cache[n];

    /* grow cache to accommodate n. */
    idx_t prev_size = cache_size;
    grow_cache(n);

    /* initialise seen array once. */
    ensure_seen_array();

    /* compute values for all indices not yet cached. */
    for (idx_t cur = prev_size; cur <= n; ++cur) {
        /* start a new mex computation */
        ++seen_tag;
        if (seen_tag == 0) {          /* tag overflow – reset array */
            for (size_t i = 0; i < MAX_MEX_MARK; ++i) seen[i] = 0;
            seen_tag = 1;
        }

        /* iterate over all allowed block sizes k = 1…7 */
        for (int k = 1; k <= 7 && (idx_t)k <= cur; ++k) {
            uint32_t d = octal_digits[k];
            if (!d) continue;                /* no move of this size */

            idx_t max_l = cur - (idx_t)k;
            for (idx_t l = 0; l <= max_l; ++l) {
                idx_t r = cur - (idx_t)k - l;

                /* check split rules */
                if ((l > 0 && !(d & 1)) ||
                    (r > 0 && !(d & 2)) ||
                    (l > 0 && r > 0 && !(d & 4))) {
                    continue; /* split not allowed */
                }

                seq_t x = cache[l] ^ cache[r];
                if (x < (seq_t)MAX_MEX_MARK) {
                    seen[(size_t)x] = seen_tag;
                }
            }
        }

        /* find the mex */
        size_t mex = 0;
        while (seen[mex] == seen_tag) ++mex;
        cache[cur] = (seq_t)mex;
    }

    return cache[n];
}
