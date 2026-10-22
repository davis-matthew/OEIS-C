/*
 * A354170  Odd numbers whose Collatz trajectory includes 11 odd numbers.
 * https://oeis.org/A354170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd numbers whose Collatz trajectory includes exactly 11 odd numbers.
   Implemented by brute‑force search over odd integers, counting odd terms in each
   Collatz trajectory until 1 is reached. */
#include <stdlib.h>

static seq_t *cache = NULL;        /* stored sequence values */
static idx_t cache_cap = 0;        /* allocated capacity */
static idx_t cache_len = 0;        /* number of computed terms */

static seq_t next_candidate = 1;   /* last odd integer examined */

/* Ensure that at least n terms are computed and stored in cache. */
static void ensure_terms(idx_t n) {
    const idx_t TARGET = 11;        /* desired number of odd terms */
    while (cache_len < n) {
        /* move to next odd integer */
        next_candidate += 2;
        seq_t x = next_candidate;
        idx_t odd_count = 0;
        while (1) {
            if (x & 1) odd_count++;            /* count odd term */
            if (odd_count > TARGET) break;      /* not enough */
            if (x == 1) break;                  /* reached end */
            if (x & 1) x = 3*x + 1;             /* odd step */
            else x >>= 1;                        /* even step */
        }
        if (odd_count == TARGET) {
            /* store value */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();        /* memory exhausted */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
    }
}

/* Public sequence routine requested by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined, return 0 for safety */
    ensure_terms(n);
    return cache[n - 1];
}
