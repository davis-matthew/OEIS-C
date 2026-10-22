/*
 * A372196  a(1) = 1. For n > 1, let m be the number of times that the value
 *          a(n-1) has appeared (including its appearance at a(n-1)); if m > 1
 *          then a(n) = m, otherwise a(n) = a(a(n-1)).
 * https://oeis.org/A372196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A372196
 *  Recurrence:  a(1) = 1.
 *  For n > 1, let k = a(n-1) and let m be the number of times k has
 *  appeared in a(1)..a(n-1) (including the appearance at position n-1).
 *  If m > 1 then a(n) = m, otherwise a(n) = a(k).
 *  The function is memoised for efficient repeated queries.
 */
#include <stdlib.h>
#include <string.h>

/* static storage for memoisation */
static seq_t *seq_arr   = NULL;   /* terms a(i) */
static idx_t *count_arr = NULL;   /* occurrences of each value */
static idx_t cap_seq    = 0;      /* allocated capacity of seq_arr   */
static idx_t cap_count  = 0;      /* allocated capacity of count_arr */
static idx_t max_n      = 0;      /* highest n for which a(n) is computed */

/* grow seq_arr to hold at least need elements (0..need) */
static void ensure_seq(idx_t need)
{
    if (need <= cap_seq) return;
    idx_t new_cap = cap_seq ? cap_seq << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* zero the new part */
    memset(tmp + cap_seq, 0, (new_cap - cap_seq) * sizeof(seq_t));
    seq_arr = tmp;
    cap_seq = new_cap;
}

/* grow count_arr to hold at least need indices (0..need) */
static void ensure_count(idx_t need)
{
    if (need <= cap_count) return;
    idx_t new_cap = cap_count ? cap_count << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(count_arr, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    memset(tmp + cap_count, 0, (new_cap - cap_count) * sizeof(idx_t));
    count_arr = tmp;
    cap_count = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0 */

    /* if already computed, just return */
    if (n <= max_n) return seq_arr[n];

    /* prepare storage */
    ensure_seq(n + 1);            /* need indices 0..n */
    ensure_count(n + 1);

    /* initialise the very first term when first called */
    if (max_n == 0) {
        seq_arr[1] = (seq_t)1;
        count_arr[1] = 1;
        max_n = 1;
    }

    /* compute missing terms */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t prev = seq_arr[i - 1];
        idx_t idx_prev = (idx_t)prev;            /* value used as index */
        idx_t m = count_arr[idx_prev];           /* occurrences so far */

        seq_t val;
        if (m > 1) {
            val = (seq_t)m;                      /* rule 1 */
        } else {
            val = seq_arr[idx_prev];             /* rule 2, idx_prev < i */
        }

        seq_arr[i] = val;
        idx_t idx_val = (idx_t)val;
        count_arr[idx_val] += 1;                /* update frequency */
    }
    max_n = n;
    return seq_arr[n];
}
