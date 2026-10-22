/*
 * A379692  a(1) = 1; a(n) is the most recently occurring value not among the
 *          last k terms where k is the number of times a(n-1) has occurred;
 *          if no such value exists, a(n) is the smallest unused positive
 *          integer.
 * https://oeis.org/A379692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   idx_from_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1;
   For n > 1 let k be the number of times a(n‑1) has appeared.
   a(n) is the most recent value before the last k terms that is not
   among those last k terms; if no such value exists, a(n) is the
   smallest unused positive integer. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/idx_from_seq.h"

/* --------------- static storage ------------------------------------------ */
static seq_t *seq = NULL;                 /* 1‑indexed terms array              */
static size_t seq_cap = 0;                /* allocated capacity of seq array    */
static size_t seq_len = 0;                /* number of terms computed           */

static size_t *cnt = NULL;                /* occurrence count of each value     */
static size_t cnt_cap = 0;                /* allocated capacity of cnt array    */

static bool *in_win = NULL;               /* marks whether a value is in last k */
static size_t win_cap = 0;                /* allocated capacity of in_win       */

static size_t *win_vals = NULL;           /* temporary storage of unique window values */
static size_t win_vals_cap = 0;           /* capacity of win_vals               */

static size_t next_unused = 1;            /* smallest unused positive integer   */
static size_t max_val_seen = 0;           /* largest value that has appeared     */

/* ------------------------------------------------------------------------ */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_cnt_capacity(size_t v)
{
    size_t need = v + 1;
    if (need <= cnt_cap) return;
    size_t new_cap = cnt_cap ? cnt_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    cnt = (size_t *)realloc(cnt, new_cap * sizeof(size_t));
    for (size_t i = cnt_cap; i < new_cap; ++i) cnt[i] = 0;
    cnt_cap = new_cap;
}

static void ensure_win_capacity(size_t v)
{
    size_t need = v + 1;
    if (need <= win_cap) return;
    size_t new_cap = win_cap ? win_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    bool *new_in = (bool *)realloc(in_win, new_cap * sizeof(bool));
    for (size_t i = win_cap; i < new_cap; ++i) new_in[i] = false;
    in_win = new_in;
    win_cap = new_cap;
}

static void ensure_win_vals_capacity(size_t need)
{
    if (need <= win_vals_cap) return;
    size_t new_cap = win_vals_cap ? win_vals_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    size_t *new_vals = (size_t *)realloc(win_vals, new_cap * sizeof(size_t));
    win_vals = new_vals;
    win_vals_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
static void compute_upto(idx_t n)
{
    while (seq_len < (size_t)n) {
        size_t next_pos = seq_len + 1;          /* index to be filled           */

        /* --- a(1) ------------------------------------------------------- */
        if (next_pos == 1) {
            ensure_seq_capacity(1);
            seq[1] = (seq_t)1;
            ensure_cnt_capacity(1);
            cnt[1] = 1;
            seq_len = 1;
            max_val_seen = 1;
            continue;
        }

        /* --- number of times previous term has appeared ---------------- */
        seq_t last_val = seq[seq_len];
        size_t last_idx = idx_from_seq(last_val);
        size_t k = cnt[last_idx];

        /* --- window of last k terms ------------------------------------ */
        size_t win_start = (k <= seq_len) ? (seq_len + 1 - k) : 1;
        size_t win_end   = seq_len;                /* inclusive */

        /* ensure auxiliary arrays big enough */
        ensure_win_capacity(max_val_seen);
        ensure_win_vals_capacity(k);

        size_t uniq_cnt = 0;
        for (size_t j = win_start; j <= win_end; ++j) {
            size_t v = idx_from_seq(seq[j]);
            if (!in_win[v]) {
                in_win[v] = true;
                win_vals[uniq_cnt++] = v;
            }
        }

        /* --- search most recent earlier term not in window ------------ */
        seq_t chosen = 0;                          /* 0 means none found */
        if (win_start > 1) {
            for (size_t j = win_start - 1; j >= 1; --j) {
                size_t v = idx_from_seq(seq[j]);
                if (!in_win[v]) { chosen = seq[j]; break; }
                if (j == 1) break;                 /* avoid underflow */
            }
        }

        /* clear window markers */
        for (size_t t = 0; t < uniq_cnt; ++t)
            in_win[win_vals[t]] = false;

        seq_t new_val;
        if (chosen != 0) {
            new_val = chosen;
        } else {
            /* find smallest unused positive integer */
            /* ensure cnt array is big enough for next_unused */
            ensure_cnt_capacity(next_unused);
            while (cnt[idx_from_seq((seq_t)next_unused)] > 0) ++next_unused;
            new_val = (seq_t)next_unused;
            ++next_unused;
        }

        /* --- insert new term ------------------------------------------ */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len + 1] = new_val;
        size_t new_idx = idx_from_seq(new_val);
        ensure_cnt_capacity(new_idx);
        ++cnt[new_idx];

        /* update global max and next_unused */
        if (new_idx > max_val_seen) max_val_seen = new_idx;
        ensure_cnt_capacity(next_unused);
        while (cnt[idx_from_seq((seq_t)next_unused)] > 0) ++next_unused;

        ++seq_len;
    }
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0, but safe */
    compute_upto(n);
    return seq[n];
}
