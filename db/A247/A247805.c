/*
 * A247805  Numbers in decimal representation with distinct digits, such that
 *          in German their digits are in alphabetic order.
 * https://oeis.org/A247805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 1007, u64 n <= 1008,
 *            u128 n <= 1008, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A247805: numbers with distinct digits in German
 * alphabetic order. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* German alphabetic order of the digit names */
static const unsigned char digit_order[10] = {8, 3, 1, 5, 9, 0, 6, 7, 4, 2};

/* Masks of digits that appear before (earlier) or after (later) 0 in that order */
#define EARLIER_MASK (((idx_t)1 << 8) | ((idx_t)1 << 3) | ((idx_t)1 << 1) | ((idx_t)1 << 5) | ((idx_t)1 << 9))
#define LATER_MASK   (((idx_t)1 << 6) | ((idx_t)1 << 7) | ((idx_t)1 << 4) | ((idx_t)1 << 2))

/* Comparator for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Static cache of all terms */
static seq_t cache[1024];
static size_t cache_size = 0;
static bool cache_ready = false;

/* Build the sequence once */
static void build_cache(void)
{
    if (cache_ready) return;
    size_t cnt = 0;

    /* iterate over all non‑empty subsets of the 10 digits */
    for (idx_t mask = 1; mask < ((idx_t)1 << 10); ++mask)
    {
        /* Skip subsets that contain 0, contain a later digit, and contain no earlier digit.
           Such subsets would produce a leading zero, which is not allowed. */
        if ((mask & ((idx_t)1 << 0)) &&
            ((mask & EARLIER_MASK) == 0) &&
            ((mask & LATER_MASK) != 0))
        {
            continue;
        }

        seq_t val = 0;
        for (int i = 0; i < 10; ++i)
        {
            unsigned d = digit_order[i];
            if (mask & ((idx_t)1 << d))
                val = val * (seq_t)10 + (seq_t)d;
        }
        cache[cnt++] = val;
    }

    cache_size = cnt;
    qsort(cache, cache_size, sizeof(seq_t), seq_cmp);
    cache_ready = true;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */
    build_cache();
    if (n > cache_size) return 0;   /* beyond the sequence */
    return cache[n - 1];
}
