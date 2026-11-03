/*
 * A353852  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has all distinct run-sums.
 * https://oeis.org/A353852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th nonnegative integer k whose k‑th composition in standard order
 * (graded reverse‑lexicographic, A066099) has all distinct run‑sums.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Helper: determine whether the composition corresponding to k has all
 *  distinct run‑sums.  The definition of the composition (see OEIS
 *  comments) is implemented directly.  */
static bool distinct_run_sums(idx_t k)
{
    /* At most 64 parts for 64‑bit indices – enough for all builds.   */
    uint64_t parts[64];
    size_t part_cnt = 0;
    uint64_t prev_pos = 0;

    /* Scan bits of k as they appear in the reversed binary representation. */
    for (uint64_t i = 0; (k >> i) != 0; ++i) {
        if ((k >> i) & 1ULL) {
            uint64_t pos = i + 1;                    /* 1‑based position */
            parts[part_cnt++] = pos - prev_pos;     /* part size */
            prev_pos = pos;
        }
    }

    /* If k == 0 the composition is empty – it trivially satisfies the
     * distinct‑run‑sum property. */
    if (part_cnt == 0) return true;

    /* Reverse parts to obtain the standard‑order composition. */
    for (size_t i = 0; i < part_cnt / 2; ++i) {
        uint64_t tmp = parts[i];
        parts[i] = parts[part_cnt - 1 - i];
        parts[part_cnt - 1 - i] = tmp;
    }

    /* Now walk through the composition grouping equal adjacent parts.  */
    uint64_t seen_sums[64];
    size_t seen_cnt = 0;
    uint64_t run_val = parts[0];
    uint64_t run_sum = parts[0];

    for (size_t i = 1; i < part_cnt; ++i) {
        if (parts[i] == run_val) {
            run_sum += parts[i];
        } else {
            /* Check for duplicate run sum. */
            for (size_t j = 0; j < seen_cnt; ++j)
                if (seen_sums[j] == run_sum) return false;
            seen_sums[seen_cnt++] = run_sum;
            run_val = parts[i];
            run_sum = parts[i];
        }
    }

    /* Final run must also be checked. */
    for (size_t j = 0; j < seen_cnt; ++j)
        if (seen_sums[j] == run_sum) return false;
    seen_sums[seen_cnt++] = run_sum;

    return true;
}

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*  The main sequence function.  It memoises previously found values in a
 *  dynamic array so that calls in arbitrary order remain fast.          */
seq_t a(idx_t n)
{
    /* Cache of valid k values.  The array holds idx_t values, cast to seq_t
     * on return.  */
    static idx_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static idx_t next_k = 0;   /* next candidate k to test */

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 1024;
        cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        cache_sz = 0;
    }

    /* Generate terms until we have at least n+1 entries. */
    while (cache_sz <= (size_t)n) {
        if (distinct_run_sums(next_k)) {
            if (cache_sz == cache_cap) {
                cache_cap *= 2;
                cache = (idx_t *)realloc(cache, cache_cap * sizeof(idx_t));
            }
            cache[cache_sz++] = next_k;
        }
        ++next_k;
    }

    return (seq_t)cache[n];
}
