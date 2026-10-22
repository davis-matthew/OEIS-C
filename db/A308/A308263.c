/*
 * A308263  Number of ordered factorizations of n into Lucas numbers
 *          (beginning at 2) > 1.
 * https://oeis.org/A308263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, a(n) = sum_{l in Lucas numbers >1, l|n}
 *            a(n/l). The Lucas numbers are 2, 3, 4, 7, 11, 18, … (defined by
 *            L0=2, L1=1, Lk = L_{k-1} + L_{k-2})
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  State for memoisation and for the list of Lucas numbers           */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;          /* cache[i] == a(i)                   */
static size_t cache_cap = 0;         /* allocated size of cache array       */
static idx_t  cache_max = 0;         /* largest n that has been computed   */

static uint64_t *lucas = NULL;       /* list of Lucas numbers >1            */
static size_t lucas_len = 0;         /* how many are stored                */
static uint64_t lucas_max = 0;       /* largest Lucas number stored         */

/* ------------------------------------------------------------------ */
/*  Ensure cache space for all indices 0 .. n                           */
/* ------------------------------------------------------------------ */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1); /* out of memory; abort */
    cache = tmp;

    /* initialise new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;

    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Build the list of Lucas numbers >1 up to at least n                */
/* ------------------------------------------------------------------ */
static void ensure_lucas(idx_t n)
{
    if (n <= lucas_max) return;

    /* free old list if any */
    if (lucas) free(lucas);
    lucas = NULL;
    lucas_len = 0;
    lucas_max = 0;

    uint64_t l0 = 2;  /* L0 */
    uint64_t l1 = 1;  /* L1 */

    while (l0 <= n) {
        if (l0 > 1) {          /* we only keep Lucas numbers > 1 */
            uint64_t *tmp = (uint64_t *)realloc(lucas,
                                                (lucas_len + 1) * sizeof(uint64_t));
            if (!tmp) exit(1);
            lucas = tmp;
            lucas[lucas_len++] = l0;
            lucas_max = l0;
        }
        uint64_t next = l0 + l1;
        l0 = l1;
        l1 = next;
    }
}

/* ------------------------------------------------------------------ */
/*  Main function computing a(n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for 0; safe to return 0 */

    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_cache_size(1);
        cache[1] = 1;
        cache_max = 1;
    }

    /* extend precomputation as needed */
    if (n > cache_max) {
        ensure_cache_size(n);
        ensure_lucas(n);

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (size_t j = 0; j < lucas_len; ++j) {
                uint64_t l = lucas[j];
                if (l > (uint64_t)i) break;
                if ((i % l) == 0) {
                    idx_t q = i / l;
                    sum += cache[(size_t)q];
                }
            }
            cache[(size_t)i] = sum;
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
