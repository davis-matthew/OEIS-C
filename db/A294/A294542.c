/*
 * A294542  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + 1, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequence computation
   a(n) = a(n-1) + a(n-2) + b(n-1) + 1
   with a(0)=1, a(1)=2, b(0)=3
   The sequences a() and b() are increasing complements.
*/

#include <stdlib.h>
#include <stdbool.h>

/* Static storage used for memoisation across calls */
static seq_t *a_cache = NULL;       /* array holding a(0)...a(len-1)          */
static seq_t *b_cache = NULL;       /* array holding b(0)...b(len-1)          */
static idx_t a_len = 0;             /* number of a-values stored */
static idx_t b_len = 0;             /* number of b-values stored */
static idx_t a_cap = 0;             /* allocated capacity for a_cache          */
static idx_t b_cap = 0;             /* allocated capacity for b_cache          */
static seq_t last_b = 0;            /* the largest b already generated        */

/* binary search to test whether val appears in a_cache[0 .. upto-1] */
static bool in_a(seq_t val, idx_t upto)
{
    idx_t lo = 0, hi = upto;          /* hi exclusive */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (a_cache[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < upto && a_cache[lo] == val);
}

/* ensures that a_cache and b_cache have enough space for 'need' elements */
static void ensure_cap(idx_t *cap, seq_t **buf, idx_t need)
{
    if (*cap < need) {
        idx_t new_cap = (need > 4) ? need * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(*buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* abort on allocation failure */
        *buf = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* --- initialise the sequence if needed -------------------------------- */
    if (a_len < 2) {
        ensure_cap(&a_cap, &a_cache, 2u);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_len = 2;
    }
    if (b_len < 1) {
        ensure_cap(&b_cap, &b_cache, 1u);
        b_cache[0] = (seq_t)3;
        b_len = 1;
        last_b = (seq_t)3;
    }

    /* target length of a_cache to cover a(n) */
    idx_t target_len = n + 1;

    /* --- build the sequence up to the requested index ------------------- */
    while (a_len < target_len) {
        /* we must have b[ a_len-1 ] ready for the recurrence */
        idx_t need_b_index = a_len - 1;
        while (b_len <= need_b_index) {
            /* find the next unused integer (not in a_cache[0..a_len-1]) */
            seq_t candidate = last_b + 1;
            while (in_a(candidate, a_len))
                candidate++;
            /* record it as the next b */
            ensure_cap(&b_cap, &b_cache, b_len + 1);
            b_cache[b_len++] = candidate;
            last_b = candidate;
        }

        /* compute the next a value */
        seq_t next_a = a_cache[a_len-1] + a_cache[a_len-2]
                      + b_cache[a_len-1] + (seq_t)1;
        ensure_cap(&a_cap, &a_cache, a_len + 1);
        a_cache[a_len++] = next_a;
    }

    return a_cache[n];
}
