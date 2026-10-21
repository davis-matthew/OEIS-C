/*
 * A277142  Lexicographically least sequence of nonnegative integers that
 *          avoids 4/3-powers.
 * https://oeis.org/A277142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7823, u16 n <= 7823, u32 n <= 7823, u64 n <= 7823,
 *            u128 n <= 7823, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277142: lexicographically least sequence of nonnegative
   integers avoiding 4/3‑powers.
   Computation: iterative construction, testing candidates for each new
   position by checking all potential 4/3‑powers ending at that position.
   Caching is used to allow arbitrary call order. */
/* Note: seq_t and idx_t are supplied by the build environment. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for previously computed terms */
static seq_t *seq_cache = NULL;   /* array of computed values */
static idx_t cached_len = 0;      /* number of terms stored (indices 0..cached_len-1) */
static size_t cache_cap = 0;      /* allocated capacity for seq_cache */

/* ensure seq_cache contains values up to index n (inclusive) */
static void ensure_upto(idx_t n)
{
    if (n < cached_len) {
        return; /* already computed */
    }

    /* grow capacity to a size ≥ n+1, preferably a power of 2 */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) {
        new_cap <<= 1;
    }

    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* out‑of‑memory: abort program */
            abort();
        }
        seq_cache = tmp;
    }
    cache_cap = new_cap;

    /* compute missing terms one by one */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t cand = 0;
        for (;;) {                 /* find smallest admissible value */
            bool bad = false;
            idx_t len   = i + 1;    /* current length after appending cand */
            idx_t maxk  = len / 4;  /* largest k with 4k ≤ len */

            for (idx_t k = 1; k <= maxk && !bad; ++k) {
                idx_t start        = len - 4 * k;       /* start of the 4k block */
                idx_t compare_start = start + 3 * k;   /* position of last k block */

                /* compare the first k elements with the last k elements */
                for (idx_t j = 0; j < k; ++j) {
                    seq_t left  = seq_cache[start + j];
                    seq_t right = (compare_start + j == i) ? cand
                                                            : seq_cache[compare_start + j];
                    if (left != right) {
                        break;
                    }
                    if (j == k - 1) {
                        /* all k positions matched → 4/3‑power found */
                        bad = true;
                        break;
                    }
                }
            }
            if (!bad) {
                seq_cache[i] = cand;
                break;               /* accepted candidate */
            }
            ++cand;                  /* try next candidate */
        }
    }

    cached_len = n + 1;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return seq_cache[n];
}
