/*
 * A357135  Take the k-th composition in standard order for each part k of the
 *          n-th composition in standard order; then concatenate.
 * https://oeis.org/A357135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th term of the infinite concatenated sequence
 *            obtained by selecting the n‑th composition in standard order
 *            (A066099), replacing each part k of that composition by the k‑th
 *            composition in the same order, and concatenating all parts of
 *            the resulting compositions
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---- internal state ---------------------------------------------------- */

static idx_t next_comp_to_process = 1;      /* next composition index to produce */
static size_t cache_len   = 0;               /* number of cached output elements */
static size_t cache_cap   = 0;               /* allocated capacity of cache     */
static seq_t *cache       = NULL;           /* cached output values             */

static size_t comps_len   = 0;               /* number of computed compositions */
static size_t comps_cap   = 0;               /* capacity of composition arrays   */
static seq_t **comp_parts = NULL;           /* parts array for each comp       */
static idx_t *comp_counts = NULL;           /* number of parts for each comp   */

/* ---- helpers ----------------------------------------------------------- */

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void ensure_comps_upto(idx_t k)
{
    if (k <= comps_len) return;          /* already done */

    /* grow capacity if needed */
    size_t new_cap = comps_cap ? comps_cap : 16;
    while (new_cap < (size_t)k) new_cap <<= 1;

    if (new_cap > comps_cap) {
        comp_parts  = (seq_t **)realloc(comp_parts,  new_cap * sizeof(seq_t *));
        comp_counts = (idx_t *)realloc(comp_counts, new_cap * sizeof(idx_t));
        comps_cap   = new_cap;
    }

    /* compute missing compositions sequentially */
    for (idx_t i = comps_len + 1; i <= k; ++i) {
        /* number of parts = popcount(i) */
        idx_t cnt = 0;
        idx_t tmp = i;
        while (tmp) {
            if (tmp & 1U) ++cnt;
            tmp >>= 1;
        }

        seq_t *parts = NULL;
        if (cnt > 0) parts = (seq_t *)malloc(cnt * sizeof(seq_t));

        /* collect differences between successive positions of 1's */
        tmp   = i;
        idx_t pos = 1;          /* 1‑based bit position */
        idx_t prev = 0;
        idx_t di   = 0;
        while (tmp) {
            if (tmp & 1U) {
                parts[di++] = (seq_t)(pos - prev);
                prev = pos;
            }
            ++pos;
            tmp >>= 1;
        }

        /* reverse the differences to get the composition parts */
        for (idx_t j = 0; j + j + 1 < cnt; ++j) {
            seq_t t = parts[j];
            parts[j] = parts[cnt - 1 - j];
            parts[cnt - 1 - j] = t;
        }

        comp_parts[i - 1]  = parts;
        comp_counts[i - 1] = cnt;
    }

    comps_len = k;
}

static void generate_block_for(idx_t comp_idx)
{
    ensure_comps_upto(comp_idx);
    idx_t len_cur   = comp_counts[comp_idx - 1];
    seq_t *parts_cur = comp_parts[comp_idx - 1];

    for (idx_t pi = 0; pi < len_cur; ++pi) {
        seq_t part_val = parts_cur[pi];
        /* part_val is at least 1; cast to idx_t safely for small values   */
        idx_t part_idx = (idx_t)part_val;
        ensure_comps_upto(part_idx);
        idx_t len_part = comp_counts[part_idx - 1];
        seq_t *parts_part = comp_parts[part_idx - 1];

        for (idx_t pj = 0; pj < len_part; ++pj) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = parts_part[pj];
        }
    }
}

/* ---- public function --------------------------------------------------- */

seq_t a(idx_t n)
{
    /* produce terms until we have enough to answer request */
    while (cache_len <= n) {
        generate_block_for(next_comp_to_process);
        ++next_comp_to_process;
    }
    return cache[n];
}
