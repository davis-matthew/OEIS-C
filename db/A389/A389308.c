/*
 * A389308  Lexicographically earliest sequence, where a(n+1) - a(n) is unique
 *          and if a(i) = a(n+1), i <= n, then the minimal n+1 - i is also
 *          unique.
 * https://oeis.org/A389308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest sequence where each * difference
 *            a(n+1)-a(n) is distinct, and the minimal * distance between a
 *            repeated value and any earlier occurrence * is unique
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic array helpers */
static void ensure_seq_cap(void);
static void ensure_pos_cap(void);
static void ensure_neg_cap(void);
static void ensure_gap_cap(void);
static void ensure_val_cap(void);

/* storage */
static seq_t *seq_arr   = NULL;   /* a[0..seq_len-1]   */
static size_t seq_len = 0, seq_cap = 0;

static seq_t *pos_diff = NULL;   /* positive diffs   */
static seq_t *neg_diff = NULL;   /* negative diffs (abs) */
static size_t pos_len = 0, pos_cap = 0;
static size_t neg_len = 0, neg_cap = 0;
static bool   zero_used = false;

static seq_t *used_gaps = NULL;  /* unique minimal gaps */
static size_t used_len = 0, used_cap = 0;

static seq_t *val_arr   = NULL;  /* distinct values  */
static idx_t *last_arr  = NULL;  /* last index of each value */
static size_t val_cnt = 0, val_cap = 0;

/* utility: linear search for a positive diff */
static bool diff_pos_used(seq_t d)
{
    for (size_t i = 0; i < pos_len; ++i)
        if (pos_diff[i] == d) return true;
    return false;
}

/* utility: linear search for a negative diff (abs value) */
static bool diff_neg_used(seq_t d)
{
    for (size_t i = 0; i < neg_len; ++i)
        if (neg_diff[i] == d) return true;
    return false;
}

/* utility: linear search for a used gap */
static bool gap_used(seq_t g)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_gaps[i] == g) return true;
    return false;
}

/* dynamic array growth helpers */
static void ensure_seq_cap(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}
static void ensure_pos_cap(void)
{
    if (pos_len == pos_cap) {
        size_t new_cap = pos_cap ? pos_cap * 2 : 16;
        pos_diff = (seq_t*)realloc(pos_diff, new_cap * sizeof(seq_t));
        pos_cap = new_cap;
    }
}
static void ensure_neg_cap(void)
{
    if (neg_len == neg_cap) {
        size_t new_cap = neg_cap ? neg_cap * 2 : 16;
        neg_diff = (seq_t*)realloc(neg_diff, new_cap * sizeof(seq_t));
        neg_cap = new_cap;
    }
}
static void ensure_gap_cap(void)
{
    if (used_len == used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        used_gaps = (seq_t*)realloc(used_gaps, new_cap * sizeof(seq_t));
        used_cap = new_cap;
    }
}
static void ensure_val_cap(void)
{
    if (val_cnt == val_cap) {
        size_t new_cap = val_cap ? val_cap * 2 : 16;
        val_arr  = (seq_t*)realloc(val_arr,  new_cap * sizeof(seq_t));
        last_arr = (idx_t*)realloc(last_arr, new_cap * sizeof(idx_t));
        val_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)  /* external linkage */
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe */

    while (seq_len < (size_t)n) {
        if (seq_len == 0) {   /* a(1) = 0 */
            ensure_seq_cap();
            seq_arr[seq_len++] = (seq_t)0;
            /* store value 0 */
            ensure_val_cap();
            val_arr[0] = (seq_t)0;
            last_arr[0] = (idx_t)0;
            val_cnt = 1;
            continue;
        }

        idx_t idx = (idx_t)seq_len;        /* 0‑based index of new element */
        seq_t prev = seq_arr[seq_len - 1];

        seq_t cand = 0;
        bool seen = false;
        size_t vidx = 0;   /* index in val_arr if seen */

        /* search for lexicographically smallest candidate */
        for (cand = 0; ; ++cand) {
            /* difference uniqueness */
            bool diff_used = false;
            if (cand < prev) {           /* negative difference */
                seq_t diff_abs = prev - cand;
                if (!diff_neg_used(diff_abs))
                    diff_used = false;
                else
                    diff_used = true;
            } else {                      /* cand >= prev */
                if (cand == prev) {
                    diff_used = zero_used;
                } else {
                    seq_t diff_abs = cand - prev;
                    diff_used = diff_pos_used(diff_abs);
                }
            }
            if (diff_used) continue;

            /* check if candidate has appeared before */
            seen = false;
            for (size_t i = 0; i < val_cnt; ++i) {
                if (val_arr[i] == cand) {
                    seen = true;
                    vidx = i;
                    break;
                }
            }

            if (seen) {
                /* minimal distance to previous occurrence */
                seq_t dist = (seq_t)(idx - last_arr[vidx]);
                if (gap_used(dist)) continue;  /* not unique */
            }
            /* if we reach here, candidate is acceptable */
            break;
        }

        /* store chosen candidate */
        ensure_seq_cap();
        seq_arr[seq_len++] = cand;

        /* record the difference */
        if (cand != prev) {
            if (cand > prev) {          /* positive diff */
                seq_t d = cand - prev;
                ensure_pos_cap();
                pos_diff[pos_len++] = d;
            } else {                    /* negative diff */
                seq_t d = prev - cand;
                ensure_neg_cap();
                neg_diff[neg_len++] = d;
            }
        } else {                        /* diff == 0 */
            zero_used = true;
        }

        /* handle value bookkeeping */
        if (seen) {
            /* new minimal distance must be recorded */
            seq_t dist = (seq_t)(idx - last_arr[vidx]);   /* distance to previous */
            ensure_gap_cap();
            used_gaps[used_len++] = dist;
            /* update last occurrence */
            last_arr[vidx] = idx;
        } else {
            /* new distinct value */
            ensure_val_cap();
            val_arr[val_cnt]   = cand;
            last_arr[val_cnt]  = idx;
            val_cnt++;
        }
    }

    return seq_arr[n - 1];
}
