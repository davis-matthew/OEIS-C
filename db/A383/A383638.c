/*
 * A383638  Right-truncatable happy numbers: every prefix is a happy number
 *          and no digits are zero.
 * https://oeis.org/A383638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the nth right‑truncatable happy number (no zero digits),
 *            sorted in ascending order
 * Range:     u8 none, u16 none, u32 none, u64 n <= 170, u128 n <= 170,
 *            big any n
 * Helpers:   isHappy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isHappy.h"

/* ----------------- comparator for qsort ----------------- */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* ----------------- build cache of all terms ----------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;

static void build_cache(void)
{
    size_t cap = 10;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    cache_len = 0;

    /* start with single‑digit happy numbers (non‑zero digits) */
    for (int d = 1; d <= 9; ++d)
    {
        seq_t val = (seq_t)d;
        if (isHappy(val))
        {
            cache[cache_len++] = val;
            if (cache_len == cap)
            {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
        }
    }

    /* extend each current number by appending digits 1..9 */
    for (size_t idx = 0; idx < cache_len; ++idx)
    {
        seq_t prefix = cache[idx];
        for (int d = 1; d <= 9; ++d)
        {
            seq_t cand = prefix * (seq_t)10 + (seq_t)d;
            if (isHappy(cand))
            {
                if (cache_len == cap)
                {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[cache_len++] = cand;
            }
        }
    }

    /* sort the resulting list */
    qsort(cache, cache_len, sizeof(seq_t), cmp_seq);
}

/* ----------------- public function ----------------- */
seq_t a(idx_t n)
{
    if (cache == NULL)
        build_cache();

    if (n == 0)
        return (seq_t)0;                /* not used for the sequence */
    return cache[(size_t)(n - 1)];
}
