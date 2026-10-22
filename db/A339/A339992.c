/*
 * A339992  Sums of two distinct odd cubes.
 * https://oeis.org/A339992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sums of two distinct odd cubes (OEIS A339992).
 * The function generates all odd cubes up to a fixed bound,
 * forms all pairwise sums, sorts and deduplicates them,
 * then returns the n‑th smallest value (1‑indexed).
 * This approach guarantees the correct sequence for any
 * n up to about 10 000 within the time limit. */

#include <stdlib.h>
#include <stdint.h>

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t*)p1;
    const seq_t *b = (const seq_t*)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Cache of precomputed unique sums */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;

    /* Build the cache once on the first call */
    if (cache_len == 0) {
        /* Choose enough odd numbers to produce >10 000 terms.
         * 1200 odd numbers give 719 100 pair sums, many more
         * than the number of unique sums needed. */
        const size_t odd_count = 1200;
        const size_t total_pairs = odd_count * (odd_count - 1) / 2;

        /* Allocate and compute odd cubes. */
        seq_t *cubes = (seq_t*)malloc(odd_count * sizeof(seq_t));
        if (!cubes) { return 0; }            /* allocation failure */

        for (size_t i = 0; i < odd_count; ++i) {
            seq_t odd = (seq_t)(2 * i + 1);
            cubes[i] = odd * odd * odd;      /* odd³ */
        }

        /* Allocate array for all pair sums. */
        seq_t *sums = (seq_t*)malloc(total_pairs * sizeof(seq_t));
        if (!sums) { free(cubes); return 0; }

        /* Compute sums of distinct odd cubes. */
        size_t k = 0;
        for (size_t i = 0; i < odd_count; ++i) {
            for (size_t j = i + 1; j < odd_count; ++j) {
                sums[k++] = cubes[i] + cubes[j];
            }
        }

        /* Sort the sums. */
        qsort(sums, total_pairs, sizeof(seq_t), cmp_seq);

        /* Deduplicate into a new array. */
        seq_t *uniq = (seq_t*)malloc(total_pairs * sizeof(seq_t));
        if (!uniq) { free(cubes); free(sums); return 0; }

        size_t uniq_count = 0;
        for (size_t i = 0; i < total_pairs; ++i) {
            if (uniq_count == 0 || sums[i] != sums[i - 1]) {
                uniq[uniq_count++] = sums[i];
            }
        }

        /* Clean up temporary arrays. */
        free(cubes);
        free(sums);

        /* Store the unique sorted sequence in the cache. */
        cache = uniq;
        cache_len = uniq_count;
    }

    /* Return the requested term (1‑indexed). */
    /* Caller guarantees n >= 1 and that the cache has enough terms. */
    return cache[(size_t)(n - 1)];
}
