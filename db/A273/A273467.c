/*
 * A273467  The successive numbers of digits between two "3"s in the sequence
 *          are given by the sequence itself.
 * https://oeis.org/A273467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the sequence of successive numbers of digits between
 *            consecutive '3' digits in the concatenation of the sequence
 *            itself
 * Range:     u8 n <= 90, u16 n <= 141, u32 n <= 141, u64 n <= 141,
 *            u128 n <= 141, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* global dynamic arrays */
static seq_t *seq     = NULL;     /* the sequence values */
static size_t seq_len = 0;        /* number of terms stored */
static size_t seq_cap = 0;        /* current capacity of seq */

static bool *used     = NULL;     /* used[i] = true if i already used */
static size_t used_cap = 0;        /* capacity of used */

/* state of the digit-3 gap counting */
static bool  seen_first_three = false;   /* have we seen the first '3'? */
static uint64_t digits_since_last_3 = 0; /* digits counted since the last '3' */
static size_t gap_idx  = 0;               /* how many gaps have been matched */

/* ------------------------------------------------------------ */
/* helper: enlarge 'used' array up to index n (inclusive) */
static void ensure_used_capacity(idx_t n)
{
    if ((size_t)n < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 4;
    while ((size_t)n >= new_cap) new_cap *= 2;
    bool *p = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!p) exit(1);
    memset(p + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = p;
    used_cap = new_cap;
}

/* helper: enlarge 'seq' array up to n elements */
static void ensure_seq_capacity(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (n > new_cap) new_cap *= 2;
    seq_t *p = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    seq = p;
    seq_cap = new_cap;
}

/* helper: extract decimal digits of val into out[0]..out[len-1] (LSB first) */
static void digits_of(seq_t val, unsigned char out[32], size_t *len)
{
    if (val == 0) { *len = 1; out[0] = 0; return; }
    size_t i = 0;
    seq_t tmp = val;
    while (tmp != 0) {
        out[i++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    *len = i;
}

/* ------------------------------------------------------------ */
/* test whether appending 'val' would keep the gap property */
static bool valid_candidate(seq_t val, size_t known_len)
{
    bool  local_seen = seen_first_three;
    uint64_t local_digits = digits_since_last_3;
    size_t  local_gap  = gap_idx;

    unsigned char dtab[32];
    size_t dlen;
    digits_of(val, dtab, &dlen);

    for (size_t i = 0; i < dlen; ++i) {
        uint8_t d = dtab[dlen - 1 - i];   /* most significant digit first */
        if (d == 3) {
            if (!local_seen) {
                local_seen = true;
                local_digits = 0;
            } else {
                if (local_gap < known_len) {
                    if (local_digits != seq[local_gap]) return false;
                    local_gap++;
                }
                local_digits = 0;
            }
        } else {
            local_digits++;
        }
        if (local_seen && local_gap < known_len && local_digits > seq[local_gap])
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* commit 'val' to the global state after acceptance */
static void apply_value(seq_t val)
{
    bool  local_seen  = seen_first_three;
    uint64_t local_digits = digits_since_last_3;
    size_t  local_gap = gap_idx;

    unsigned char dtab[32];
    size_t dlen;
    digits_of(val, dtab, &dlen);

    for (size_t i = 0; i < dlen; ++i) {
        uint8_t d = dtab[dlen - 1 - i];
        if (d == 3) {
            if (!local_seen) {
                local_seen = true;
                local_digits = 0;
            } else {
                if (local_gap < seq_len) local_gap++;
                local_digits = 0;
            }
        } else {
            local_digits++;
        }
    }

    seen_first_three   = local_seen;
    digits_since_last_3 = local_digits;
    gap_idx            = local_gap;
}

/* ------------------------------------------------------------ */
/* grow sequence up to at least n terms */
static void build_upto(idx_t n)
{
    while (seq_len < (size_t)n) {
        idx_t cand = 0;
        for (;;) {
            seq_t val = (seq_t)cand;
            ensure_used_capacity((size_t)cand);
            if (used[(size_t)cand]) { cand++; continue; }
            if (!valid_candidate(val, seq_len)) { cand++; continue; }
            /* accept candidate */
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = val;
            used[(size_t)cand] = true;
            apply_value(val);
            break;
        }
    }
}

/* ------------------------------------------------------------ */
/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 1 */
    build_upto((size_t)n);
    return seq[(size_t)n - 1];
}
