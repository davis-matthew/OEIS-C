/*
 * A247812  Numbers in decimal representation with distinct digits, such that
 *          in Slovak their digits are in alphabetic order.
 * https://oeis.org/A247812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 950, u64 n <= 960,
 *            u128 n <= 960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all subsets of the digits 0...9, ordering the chosen digits
   by the Slovak alphabet order (9,2,1,0,8,5,7,6,4,3).  The resulting numbers are
   collected, sorted numerically, and stored in a static cache.  The cache is built
   on the first call.  Subsequent calls return the cached value or 0 for out of
   bounds indices.  This yields the OEIS sequence A247812. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int compare_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache to hold the precomputed sequence */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Build cache on first call */
    if (cache == NULL) {
        /* The maximum possible number of terms is 960 */
        cache = (seq_t *)malloc(960 * sizeof(seq_t));
        if (cache == NULL) return 0;          /* OOM fallback */

        /* order_digits[r] gives the digit corresponding to rank r (0..9) */
        const int order_digits[10] = {9, 2, 1, 0, 8, 5, 7, 6, 4, 3};
        /* mask of digits 9,2,1 used to detect duplicates when 0 is present */
        const idx_t mask_921 = ((idx_t)1 << 9) | ((idx_t)1 << 2) | ((idx_t)1 << 1);

        size_t pos = 0;
        /* iterate over all non‑empty subsets of {0..9} */
        for (idx_t mask = 1; mask < (1ULL << 10); ++mask) {
            /* Skip duplicate subsets containing 0 but no 9,2,1 except {0} itself */
            if (mask & ((idx_t)1 << 0)) {
                if (mask != ((idx_t)1 << 0) && !(mask & mask_921))
                    continue;
            }

            /* Build the digit sequence sorted by Slovak order */
            seq_t val = 0;
            for (int rank = 0; rank < 10; ++rank) {
                int d = order_digits[rank];
                if (mask & ((idx_t)1 << d)) {
                    val = val * 10 + (seq_t)d;
                }
            }
            cache[pos++] = val;
        }

        /* pos should be exactly 960 */
        cache_size = pos;
        /* Sort the cache numerically */
        qsort(cache, cache_size, sizeof(seq_t), compare_seq_t);
    }

    if (n == 0 || n > cache_size) return 0;
    return cache[n - 1];
}
