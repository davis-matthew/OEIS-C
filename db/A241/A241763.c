/*
 * A241763  Months (on the Gregorian Calendar), numbered sequentially starting
 *          with January 2000, that begin on a Sunday.
 * https://oeis.org/A241763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Gregorian months that start on Sunday.
   Months are numbered sequentially starting with January 2000;
   a(n) is the month number (1 = January 2000) of the n‑th month that
   begins on Sunday.  */

/* The routine keeps a static cache of month numbers that start on Sunday.
   It generates months incrementally, so calls for arbitrary n are
   answered in O(n).  The algorithm uses only integer arithmetic. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array() */

static seq_t *sunday_months = NULL;   /* cache of month numbers */
static size_t cache_len = 0;          /* number of cached entries   */
static size_t cache_cap = 0;          /* allocated capacity         */

/* Ensure the cache can hold at least 'needed' entries.  */
static void cache_ensure(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache)
        abort();   /* allocation failed */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = sunday_months[i];
    if (sunday_months)
        free(sunday_months);
    sunday_months = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th month number that starts on Sunday (n ≥ 1). */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* invalid, but safe */

    /* state for month generation (persistent across calls) */
    static unsigned month_num = 1;   /* 1 for January 2000 */
    static int year = 2000;          /* current Gregorian year */
    static int m_idx = 0;            /* month index 0‑11 (Jan‑Dec) */
    static int dow = 6;              /* day of week: 0=Sunday … 6=Saturday;
                                       Jan 2000 is Saturday */

    /* Generate months until we have at least n entries cached. */
    while (cache_len < n) {
        /* If this month starts on Sunday, cache its number. */
        if (dow == 0) {
            cache_ensure(cache_len + 1);
            sunday_months[cache_len] = (seq_t)month_num;
            ++cache_len;
        }

        /* Determine length of the current month. */
        int month_len;
        switch (m_idx) {
            case 0: month_len = 31; break;                    /* Jan */
            case 1: month_len = ((year % 4 == 0 && year % 100 != 0) ||
                                 (year % 400 == 0)) ? 29 : 28; break;  /* Feb */
            case 2: month_len = 31; break;                    /* Mar */
            case 3: month_len = 30; break;                    /* Apr */
            case 4: month_len = 31; break;                    /* May */
            case 5: month_len = 30; break;                    /* Jun */
            case 6: month_len = 31; break;                    /* Jul */
            case 7: month_len = 31; break;                    /* Aug */
            case 8: month_len = 30; break;                    /* Sep */
            case 9: month_len = 31; break;                    /* Oct */
            case 10: month_len = 30; break;                   /* Nov */
            case 11: month_len = 31; break;                   /* Dec */
            default: month_len = 31; break;                   /* safety */
        }

        /* Advance to next month. */
        dow = (dow + month_len) % 7;
        ++m_idx;
        if (m_idx == 12) {
            m_idx = 0;
            ++year;
        }
        ++month_num;
    }

    return sunday_months[n - 1];
}
