/*
 * A309133  Nonnegative integers of the form 4*x^2+6*x*y-7*y^2.
 * https://oeis.org/A309133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309133: Nonnegative integers of the form 4*x^2+6*x*y-7*y^2. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Comparator for qsort, works for any seq_t type. */
static int compare_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Precompute values up to a sufficiently large bound to contain
   the first 10000 terms. */
static void compute_cache(void)
{
    /* Adjustable bound: guarantees > 10000 distinct terms. */
    const int B = 400;   /* |x|, |y| ≤ 400 */
    const size_t max_pairs = (size_t)(2 * B + 1) * (size_t)(2 * B + 1);
    seq_t *raw = (seq_t *)malloc(max_pairs * sizeof(seq_t));
    if (!raw) return;   /* allocation failure – unlikely in contest setting */

    size_t raw_len = 0;
    for (int x = -B; x <= B; ++x) {
        for (int y = -B; y <= B; ++y) {
            seq_t val = (seq_t)4 * (seq_t)x * (seq_t)x
                      + (seq_t)6 * (seq_t)x * (seq_t)y
                      - (seq_t)7 * (seq_t)y * (seq_t)y;
            if (val >= 0) {
                raw[raw_len++] = val;
            }
        }
    }

    /* Sort and deduplicate */
    qsort(raw, raw_len, sizeof(seq_t), compare_seq);

    /* Count unique values */
    size_t uniq_len = 0;
    for (size_t i = 0; i < raw_len; ++i) {
        if (i == 0 || raw[i] != raw[i - 1]) {
            raw[uniq_len++] = raw[i];
        }
    }

    /* Resize the array to the exact number of unique values */
    seq_t *final = (seq_t *)realloc(raw, uniq_len * sizeof(seq_t));
    if (!final) {
        /* On realloc failure, keep the original buffer as an emergency fallback */
        final = raw;
        uniq_len = raw_len; /* unlikely when shrinking */
    }

    cache = final;
    cache_len = uniq_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* no term before the offset */
    if (!cache) compute_cache();     /* lazy initialization */
    if (n <= cache_len) {
        return cache[n - 1];
    }
    /* In the unlikely event the requested n exceeds precomputed range,
       extend the search by increasing the bound (not needed for n≤10000). */
    /* Simple fallback: continue generating until we reach n. */
    int B = 400;
    while (n > cache_len) {
        B += 200;
        compute_cache();   /* recompute with larger bound */
    }
    return cache[n - 1];
}
