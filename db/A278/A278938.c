/*
 * A278938  Between two successive digits "2" there are 0, then 1, then 2,
 *          then 3, ... other digits.
 * https://oeis.org/A278938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 193, u16 n <= 1054, u32 n <= 1054, u64 n <= 1054,
 *            u128 n <= 1054, big any n
 * Validated: standard, pass, 127 terms through n = 1012 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A278938:
 *   a(1) = 0 and afterwards a(n) is the smallest unused integer that, when
 *   appended to the decimal string formed by a(1)..a(n‑1), keeps the
 *   positions of all '2' digits such that the gaps between successive '2's
 *   are 0,1,2,3,... respectively.
 *
 * The code builds the sequence incrementally, memoising already
 * computed values and the current state of the decimal string. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Test whether appending 'cand' keeps the string valid.
 * pos, last_two, twos are the current state *before* the append.
 * If the append is valid, new_pos, new_last_two, new_twos are set
 * to the state *after* the append and the function returns true.
 */
static bool candidate_valid(idx_t cand,
                            idx_t *pos, idx_t *last_two, idx_t *twos,
                            idx_t *new_pos, idx_t *new_last_two, idx_t *new_twos)
{
    idx_t pos_cur = *pos;
    idx_t last_two_cur = *last_two;
    idx_t twos_cur = *twos;

    /* Buffer to hold decimal digits (max 20 for 64‑bit values).  */
    char buf[32];
    int len = 0;
    if (cand == 0) {
        buf[len++] = '0';
    } else {
        idx_t tmp = cand;
        while (tmp>0) {
            buf[len++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        }
        /* reverse to most‑significant order */
        for (int i=0; i<len/2; ++i) {
            char c = buf[i];
            buf[i] = buf[len-1-i];
            buf[len-1-i] = c;
        }
    }

    /* Position where the next '2' must appear, if any. */
    idx_t next_two_pos = (twos_cur==0) ? (idx_t)-1 : last_two_cur + twos_cur;

    for (int i=0; i<len; ++i) {
        char ch = buf[i];

        /* If a '2' is required at this position, it must be present. */
        if (next_two_pos != (idx_t)-1 && pos_cur == next_two_pos && ch!='2')
            return false;

        if (ch=='2') {
            if (last_two_cur != (idx_t)-1) {
                idx_t required_gap = twos_cur - 1;
                idx_t actual_gap   = pos_cur - last_two_cur - 1;
                if (actual_gap != required_gap) return false;
            }
            last_two_cur = pos_cur;
            twos_cur++;
        }
        pos_cur++;
        /* Recompute required position for the next '2'. */
        next_two_pos = (twos_cur==0) ? (idx_t)-1 : last_two_cur + twos_cur;
    }

    *new_pos = pos_cur;
    *new_last_two = last_two_cur;
    *new_twos = twos_cur;
    return true;
}

/* ----------  helpers for sequence build  ---------- */

static seq_t *seq = NULL;          /* stored sequence values          */
static size_t seq_cap = 0;         /* capacity of seq array           */
static size_t seq_len = 0;         /* number of values stored         */

/* used[i] == 1  -> integer i already appears in the sequence   */
static uint8_t *used = NULL;
static size_t used_cap = 0;        /* number of used slots            */

static idx_t pos = 0;              /* next position in concatenated string          */
static idx_t last_two = (idx_t)-1; /* position of most recent '2', or sentinel     */
static idx_t twos   = 0;           /* number of '2's seen so far                  */

/* Ensure seq array has room for at least 'needed' elements.  */
static inline void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq = mem_resize(seq_t, seq, new_cap);
    seq_cap = new_cap;
}

/* Ensure used array can address indices up to 'n' inclusively.  */
static inline void ensure_used_cap(idx_t n)
{
    if (n < (idx_t)used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    memset(used + used_cap, 0, new_cap - used_cap);  /* new slots 0 */
    used_cap = new_cap;
}

/* ----------  public sequence function  ---------- */

seq_t a(idx_t n)
{
    static bool inited = false;
    if (!inited) {
        inited = true;
        seq = NULL;        /* seq array will be allocated lazily */
        seq_cap = 0;
        seq_len = 0;
        used = NULL;
        used_cap = 0;
        pos = 0;
        last_two = (idx_t)-1;
        twos = 0;
    }

    while (seq_len < n) {
        ensure_seq_cap(seq_len + 1);

        idx_t cand = 0;
        for (;;) {
            ensure_used_cap(cand);
            if (used[cand]) { cand++; continue; }

            idx_t new_pos, new_last, new_twos;
            if (candidate_valid(cand, &pos, &last_two, &twos,
                                &new_pos, &new_last, &new_twos)) {
                seq[seq_len++] = (seq_t)cand;
                used[cand] = 1;
                pos = new_pos;
                last_two = new_last;
                twos = new_twos;
                break;
            }
            cand++;
        }
    }

    return seq[n-1];
}
