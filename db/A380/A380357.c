/*
 * A380357  a(0) = 0, a(1) = 1, and for any n > 0, if a(n) - sign(a(n)) is not
 *          yet in the sequence then a(n+1) = a(n) - sign(a(n)), otherwise
 *          a(n+1) is the least integer (in absolute value) of opposite sign
 *          to a(n), at square distance from a(n) and not yet in the sequence.
 * https://oeis.org/A380357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 185, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _sign
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A380357
   a(0) = 0, a(1) = 1.
   For n > 0, let t = a(n). If t - sign(t) is not yet in the sequence,
   then a(n+1) = t - sign(t).  Otherwise a(n+1) is the least integer
   (in absolute value) of opposite sign to t, at square distance from t
   and not yet in the sequence.
   Here sign(x) =  1  if x > 0, -1 if x < 0, 0 if x == 0. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_sign.h"

/* supported types for seq_t and idx_t are provided by the build */
static size_t _seq_cap = 0, _seq_len = 0;
static size_t _seen_cap = 0, _seen_len = 0;
static seq_t *_seq = NULL;
static seq_t *_seen = NULL;

/* ensure capacity for sequence array */
static void _ensure_seq(size_t needed)
{
    if (_seq_cap >= needed) return;
    size_t newcap = _seq_cap ? _seq_cap : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_seq, newcap * sizeof(seq_t));
    if (!tmp) abort();
    _seq = tmp;
    _seq_cap = newcap;
}

/* ensure capacity for seen array */
static void _ensure_seen(size_t needed)
{
    if (_seen_cap >= needed) return;
    size_t newcap = _seen_cap ? _seen_cap : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_seen, newcap * sizeof(seq_t));
    if (!tmp) abort();
    _seen = tmp;
    _seen_cap = newcap;
}

/* check if value has already appeared */
static bool _is_seen(seq_t v)
{
    for (size_t i = 0; i < _seen_len; ++i)
        if (_seen[i] == v) return true;
    return false;
}

/* add value to seen set, assuming not present */
static void _add_seen(seq_t v)
{
    if (_is_seen(v)) return;
    _ensure_seen(_seen_len + 1);
    _seen[_seen_len++] = v;
}

seq_t a(idx_t n)
{
    /* initialize sequence cache if empty */
    if (_seq_len == 0) {
        _ensure_seq(2);
        _ensure_seen(2);
        _seq[0] = (seq_t)0;
        _seq[1] = (seq_t)1;
        _seq_len = 2;
        _seen_len = 0;
        _add_seen(_seq[0]);
        _add_seen(_seq[1]);
    }

    /* if n already computed, return it */
    if ((idx_t)_seq_len > n)
        return _seq[(size_t)n];

    /* compute terms up to n */
    for (idx_t i = _seq_len; i <= n; ++i) {
        seq_t t = _seq[i - 1];
        int s = _sign(t);
        seq_t next = t - (seq_t)s;          /* candidate t - sign(t) */
        if (s == 0) {                       /* should not happen after init */
            /* treat as seen; fallback to square jumps with sign 1 */
            s = 1;
            next = t - (seq_t)s;
        }
        if (!_is_seen(next)) {
            /* simple step works */
        } else {
            /* find least absolute integer opposite sign at square distance */
            seq_t cand;
            int need_sign = -s;
            for (unsigned k = 1; ; ++k) {
                seq_t square = (seq_t)k * (seq_t)k;
                cand = t - (seq_t)s * square;
                if (_sign(cand) != need_sign) continue;
                if (!_is_seen(cand)) break;
            }
            next = cand;
        }
        _ensure_seq(_seq_len + 1);
        _seq[_seq_len++] = next;
        _ensure_seen(_seen_len + 1);
        _seen[_seen_len++] = next;
    }
    return _seq[(size_t)n];
}
