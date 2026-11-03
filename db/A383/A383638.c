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
 * Validated: full, pass, 170 terms through n = 170 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ----------------- helper: check if a number is happy ----------------- */
static bool isHappy(seq_t n)
{
    seq_t seen[20];
    size_t seen_count = 0;
    seq_t x = n;

    while (x != (seq_t)1)
    {
        /* detect cycle */
        for (size_t i = 0; i < seen_count; ++i)
        {
            if (seen[i] == x)
                return false;
        }
        seen[seen_count++] = x;

        /* sum of squares of digits */
        seq_t sum = (seq_t)0;
        seq_t tmp = x;
        while (tmp > (seq_t)0)
        {
            seq_t d = tmp % (seq_t)10;
            sum += d * d;
            tmp /= (seq_t)10;
        }
        x = sum;
    }
    return true;
}

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
    cache = mem_new(seq_t, cap);
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
                cache = mem_resize(seq_t, cache, cap);
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
                    cache = mem_resize(seq_t, cache, cap);
                }
                cache[cache_len++] = cand;
            }
        }
    }

    /* sort the resulting list */
    mem_qsort(cache, cache_len, sizeof(seq_t), cmp_seq);
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
