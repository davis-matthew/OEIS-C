/*
 * A270263  Integers divisible by the last row of their "upside-down" triangle
 *          (see comments).
 * https://oeis.org/A270263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer >=10 such that the integer is divisible
 *            by the last row of its "upside-down" triangle, where the
 *            triangle is obtained by repeatedly taking absolute differences
 *            of adjacent decimal digits until only one digit remains
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* compute the last row of the "upside-down" triangle for n */
static seq_t
compute_last_row(seq_t n)
{
    /* maximum number of decimal digits for 128‑bit integer is < 40, so 64 is safe */
    uint8_t digits[64];
    size_t len = 0;
    seq_t tmp = n;

    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to obtain most significant digit first */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t x = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = x;
        }
    }

    /* iteratively compute absolute differences until one digit remains */
    while (len > 1) {
        size_t new_len = len - 1;
        for (size_t i = 0; i < new_len; ++i) {
            uint8_t a = digits[i];
            uint8_t b = digits[i + 1];
            digits[i] = (a > b) ? (a - b) : (b - a);
        }
        len = new_len;
    }
    return (seq_t)digits[0];
}

/* global cache storage for the sequence */
static seq_t *seq_cache       = NULL;
static size_t seq_cache_len   = 0;
static size_t seq_cache_cap   = 0;
static seq_t  next_check      = 10;

/* enlarge the cache if necessary */
static void
ensure_cache(size_t needed)
{
    if (needed <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = mem_resize(seq_t, seq_cache, new_cap);
    if (!new_arr)
        abort();              /* insufficient memory */
    seq_cache     = new_arr;
    seq_cache_cap = new_cap;
}

/* fill the cache up to the n‑th term (1‑based) */
static void
build_up_to(size_t n)
{
    if (n <= seq_cache_len)
        return;

    ensure_cache(n);
    for (seq_t cand = next_check; seq_cache_len < n; ++cand) {
        seq_t last = compute_last_row(cand);
        if (last != 0 && cand % last == 0) {
            seq_cache[seq_cache_len++] = cand;
        }
    }
    next_check = seq_cache[seq_cache_len - 1] + 1;
}

/* public function required by the tester */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined; return 0 for safety */
    build_up_to((size_t)n);
    return seq_cache[n - 1];
}
