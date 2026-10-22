/*
 * A317018  Sequence of distinct signed integers such that a(1) = 0 and for
 *          any n > 0, the negabinary representation of a(n+1) differ by
 *          exactly one digit from the negabinary representation of a(n) and
 *          has the smallest possible absolute value (in case of a tie, choose
 *          the integer with the rightmost difference).
 * https://oeis.org/A317018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 103, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _hash_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1, a(n) is the smallest (by absolute value)
   unvisited integer whose negabinary representation differs from a(n-1)
   in exactly one digit.  When several candidates have the same absolute
   value the one whose differing digit is the rightmost (i.e., at the
   lowest position) is chosen.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_hash_seq_t.h"

static seq_t *hash_keys = NULL;
static bool   *hash_used = NULL;
static idx_t    hash_cap  = 0;
static idx_t    hash_used_cnt = 0;
static unsigned long long hash_mask = 0;

static void _hash_rehash(idx_t new_cap)
{
    idx_t cap = 1;
    while (cap < new_cap) cap <<= 1;
    seq_t *new_keys = (seq_t *)malloc(cap * sizeof(seq_t));
    bool   *new_used = (bool   *)calloc(cap, 1);
    unsigned long long new_mask = cap - 1;
    if (!new_keys || !new_used) abort();
    for (idx_t i = 0; i < hash_cap; ++i) {
        if (!hash_used[i]) continue;
        seq_t v = hash_keys[i];
        unsigned long long h = _hash_seq_t(v) & new_mask;
        while (new_used[h]) h = (h + 1) & new_mask;
        new_used[h] = true;
        new_keys[h] = v;
    }
    free(hash_keys);
    free(hash_used);
    hash_keys = new_keys;
    hash_used = new_used;
    hash_cap  = cap;
    hash_mask = new_mask;
}

static void _hash_init(idx_t min_cap)
{
    _hash_rehash(min_cap * 4);      /* keep load factor < 0.25 */
    hash_used_cnt = 0;
}

static bool _hash_exists(seq_t v)
{
    if (!hash_cap) return false;
    unsigned long long h = _hash_seq_t(v) & hash_mask;
    while (hash_used[h]) {
        if (hash_keys[h] == v) return true;
        h = (h + 1) & hash_mask;
    }
    return false;
}

static void _hash_insert(seq_t v)
{
    if (!hash_cap) _hash_init(8);
    if ((hash_used_cnt + 1) * 4 > hash_cap) _hash_rehash(hash_cap * 2);
    unsigned long long h = _hash_seq_t(v) & hash_mask;
    while (hash_used[h]) h = (h + 1) & hash_mask;
    hash_used[h] = true;
    hash_keys[h] = v;
    ++hash_used_cnt;
}

/*--- convert a negabinary number to its digits ------------------------------------*/
#define MAX_NEG_BITS 200
static void negabinary_digits(seq_t v, int *bits, size_t *len)
{
    if (v == 0) {
        bits[0] = 0;
        *len = 1;
        return;
    }
    bits[0] = (int)((v & 1) ? 1 : 0);     /* use mask for safety */
    v = (v - (v & 1)) / (-2);
    size_t l = 1;
    while (v != 0 && l < (size_t)MAX_NEG_BITS) {
        bits[l] = (int)((v & 1) ? 1 : 0);
        v = (v - (v & 1)) / (-2);
        ++l;
    }
    *len = l;
}

/*--- sequence storage ---------------------------------------------------*/
static seq_t *seq_vals = NULL;
static idx_t  seq_cap  = 0;
static idx_t  seq_len  = 0;

static void _seq_ensure_capacity(idx_t needed)
{
    if (seq_cap < needed) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
}

static void _seq_compute(idx_t upto)
{
    if (seq_len == 0) {
        _seq_ensure_capacity(1);
        seq_vals[0] = 0;
        seq_len = 1;
        _hash_init(8);
        _hash_insert(0);
    }
    while (seq_len < upto) {
        seq_t curr = seq_vals[seq_len - 1];
        /* convert curr to negabinary digits */
        int  bits[MAX_NEG_BITS];
        size_t len;
        negabinary_digits(curr, bits, &len);
        seq_t abs_curr = curr >= 0 ? curr : -curr;

        seq_t best_val   = 0;
        seq_t best_abs   = 0;
        idx_t best_pos   = 0;
        bool  has_best   = false;

        seq_t pow_neg = 1;   /* (-2)^pos, pos = 0 */

        /* examine positions until we guarantee no better candidate can exist */
        for (idx_t pos = 0; ; ++pos) {
            int bit = (pos < (idx_t)len) ? bits[pos] : 0;
            seq_t delta = (bit == 0 ? pow_neg : -pow_neg);
            seq_t cand = curr + delta;

            if (!_hash_exists(cand)) {
                seq_t absval = cand >= 0 ? cand : -cand;
                if (!has_best || absval < best_abs ||
                    (absval == best_abs && pos < best_pos)) {
                    best_val = cand;
                    best_abs = absval;
                    best_pos = pos;
                    has_best = true;
                }
            }

            /* Estimate lower bound for future positions:
               For any position >= pos+1 the power magnitude only increases. */
            seq_t abs_pow_neg = pow_neg >= 0 ? pow_neg : -pow_neg;
            seq_t lower = abs_curr > abs_pow_neg ? abs_curr - abs_pow_neg
                                                  : abs_pow_neg - abs_curr;

            if (has_best && lower > best_abs) break;  /* no better candidate farther */

            /* safety limit: stop at a reasonable depth */
            if (pos >= (idx_t)len + 70) break;

            /* next power of (-2) */
            pow_neg = pow_neg * (seq_t)(-2);
        }

        if (!has_best) {
            /* this should never happen for the sequence definition */
            best_val = 0;
        }

        _seq_ensure_capacity(seq_len + 1);
        seq_vals[seq_len++] = best_val;
        _hash_insert(best_val);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe */
    _seq_compute(n);
    return seq_vals[n - 1];
}
