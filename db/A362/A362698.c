/*
 * A362698  For n > 1, if n appears in the sequence, a(n) = a(n-1) - n if
 *          nonnegative and not already in the sequence, otherwise a(n) =
 *          a(n-1) + n. Otherwise a(n+1) = a(n)/(n+1) if (n+1)|a(n), otherwise
 *          a(n)*(n+1), a(1) = 1 and a(2) = 1*2.
 * https://oeis.org/A362698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 24, u64 n <= 40, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A362698 recurrence:
 * a(1) = 1.
 * For n > 1:
 *   if n already appears as a term a(k) (k < n),
 *        let prev = a(n‑1).  If prev ≥ n and (prev − n) has not appeared before,
 *          a(n) = prev − n;  otherwise a(n) = prev + n.
 *   otherwise (n has not appeared yet),
 *        if prev % n == 0, a(n) = prev / n;  else a(n) = prev * n.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* dynamic storage for the sequence   */
static seq_t *seq          = NULL; /* 1‑based: seq[0] unused */
static idx_t  seq_cap      = 0;
static idx_t  seq_len      = 0;

/* array to record which small integers (up to present_small_cap) have
 * already appeared as sequence terms                                   */
static bool *present_small      = NULL; /* 1‑based indexing for values */
static idx_t present_small_cap  = 0;

/* ensure seq can hold indices up to 'needed' (i.e. capacity > needed)   */
static void ensure_seq_capacity(idx_t needed)
{
    if (seq == NULL) {
        seq_cap = 1;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (!seq) abort();
    }
    while (seq_cap <= needed) {
        seq_cap <<= 1;
        if (seq_cap == 0) seq_cap = 1;
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * seq_cap);
        if (!seq) abort();
    }
}

/* ensure present_small can index values up to 'needed' (capacity > needed) */
static void ensure_present_small_capacity(idx_t needed)
{
    if (present_small == NULL) {
        present_small_cap = 0;
        present_small = (bool *)calloc(1, sizeof(bool)); /* index 0 */
        if (!present_small) abort();
    }
    if ((idx_t)present_small_cap >= needed) return;
    idx_t old_cap   = present_small_cap;
    idx_t new_cap   = (present_small_cap < 8 ? 8 : present_small_cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    bool *tmp = (bool *)realloc(present_small, sizeof(bool) * (new_cap + 1));
    if (!tmp) abort();
    /* zero newly allocated slots */
    memset(tmp + old_cap + 1, 0, (new_cap - old_cap) * sizeof(bool));
    present_small = tmp;
    present_small_cap = new_cap;
}

/* check if a value 'v' has already appeared in the sequence */
static bool seen_value(seq_t v)
{
    /* linear scan – acceptable for n <= 10000 */
    for (idx_t j = 1; j <= seq_len; ++j) {
        if (seq[j] == v) return true;
    }
    return false;
}

/* main function computing the nth term */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – shouldn't occur */

    ensure_seq_capacity(n);
    ensure_present_small_capacity(n);

    /* initialise first term if not yet done */
    if (seq_len == 0) {
        seq[1] = 1;
        seq_len = 1;
        if (1 <= present_small_cap) present_small[1] = true;
    }

    /* compute terms up to n if necessary */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        seq_t cur;
        bool  n_exists = false;

        /* check whether i already appears as a term */
        if (i <= present_small_cap && present_small[i]) {
            n_exists = true;
        } else {
            /* full scan of previous terms */
            for (idx_t j = 1; j <= seq_len; ++j) {
                if (seq[j] == (seq_t)i) {
                    n_exists = true;
                    break;
                }
            }
            /* if found, also mark it in present_small if within range */
            if (n_exists && i <= present_small_cap) present_small[i] = true;
        }

        if (n_exists) {
            /* n appears: try subtraction */
            if (prev >= (seq_t)i) {
                seq_t cand = prev - (seq_t)i;
                if (!seen_value(cand))
                    cur = cand;
                else
                    cur = prev + (seq_t)i;
            } else {
                cur = prev + (seq_t)i;
            }
        } else {
            /* n has not appeared yet */
            if ((prev % (seq_t)i) == 0)
                cur = prev / (seq_t)i;
            else
                cur = prev * (seq_t)i;
        }

        seq[i] = cur;
        /* record the new value in present_small if it lies within range */
        if (cur <= (seq_t)present_small_cap) present_small[(idx_t)cur] = true;
    }

    seq_len = n;
    return seq[n];
}
