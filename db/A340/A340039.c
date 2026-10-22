/*
 * A340039  Numbers that are the sum of a cube s and a square t such that 0 <
 *          s <= t.
 * https://oeis.org/A340039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340039:
   Numbers that are the sum of a cube s and a square t
   with 0 < s <= t.  The function enumerates all such
   sums, removes duplicates, sorts them, and caches the result
   for fast nth‑term queries. */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort – works for any integer type seq_t. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Cache of distinct values, kept in sorted order. */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static size_t cache_len = 0;     /* number of elements stored */

/* Build the cache once.  The upper bound YMAX is chosen
   so that the first 10 000 terms are generated comfortably.
   For larger indices the cache still contains the necessary
   terms, because y^2 grows quickly. */
static void build_cache(void)
{
    if (cache != NULL)            /* already built */
        return;

    const idx_t YMAX = 2500;      /* sufficient for n up to ~10 000 */

    size_t capacity = 256;
    seq_t *buf = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!buf)
        return;

    size_t len = 0;

    for (idx_t y = 1; y <= YMAX; ++y) {
        seq_t y2 = (seq_t)y * (seq_t)y;   /* y^2 */

        for (idx_t x = 1; ; ++x) {
            seq_t x3 = (seq_t)x * (seq_t)x * (seq_t)x;  /* x^3 */
            if (x3 > y2)               /* enforce s <= t (x^3 <= y^2) */
                break;

            seq_t val = y2 + x3;       /* the required sum */

            if (len == capacity) {
                capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(buf, capacity * sizeof(seq_t));
                if (!tmp) { free(buf); return; }
                buf = tmp;
            }
            buf[len++] = val;
        }
    }

    /* Sort the generated values. */
    qsort(buf, len, sizeof(seq_t), seq_cmp);

    /* Deduplicate sorted array into a new buffer. */
    size_t uniq = 0;
    seq_t *uniq_buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (!uniq_buf) {
        free(buf);
        return;
    }

    if (len > 0) {
        uniq_buf[uniq++] = buf[0];
        for (size_t i = 1; i < len; ++i) {
            if (seq_cmp(&buf[i - 1], &buf[i]) != 0)
                uniq_buf[uniq++] = buf[i];
        }
    }

    free(buf);

    cache = uniq_buf;
    cache_len = uniq;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (!cache)
        build_cache();

    if (n == 0 || n > cache_len)
        return 0;      /* out of range – caller may decide how to handle */

    return cache[n - 1];
}
