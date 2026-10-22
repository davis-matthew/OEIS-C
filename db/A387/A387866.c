/*
 * A387866  Numbers that can be split into two numbers whose products of
 *          digits are equal.
 * https://oeis.org/A387866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 4638, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A387866:  numbers that can be split into
   two (non‑empty) parts whose products of digits are equal.
   The sequence is generated lazily and memoised. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

/* static cache for the computed sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_candidate = 10;        /* start after 10 */

/* helper: test whether a single number belongs to the sequence */
static bool is_term(seq_t x)
{
    unsigned char digits[50];
    size_t len = 0;
    get_digits(x, digits, &len);
    if (len <= 1)                /* at least two digits needed */
        return false;

    seq_t pref[50];
    seq_t suff[51];              /* suff[i] = product of digits[i .. len-1] */

    /* prefix products */
    for (size_t i = 0; i < len; ++i) {
        if (i == 0)
            pref[i] = digits[i];
        else
            pref[i] = pref[i - 1] * digits[i];
    }

    /* suffix products, computed from the end */
    for (size_t i = len; i > 0; --i) {
        size_t idx = i - 1;
        if (i == len)
            suff[idx] = digits[idx];
        else
            suff[idx] = suff[idx + 1] * digits[idx];
    }

    /* check all split positions */
    for (size_t split = 1; split < len; ++split) {
        if (pref[split - 1] == suff[split])
            return true;
    }
    return false;
}

/* extend the cache until it contains at least `target` terms */
static void ensure_cache(size_t target)
{
    while (seq_len < target) {
        seq_t cand = (seq_t)next_candidate++;
        if (is_term(cand)) {
            if (seq_len == seq_cap) {            /* grow the array */
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *new_ptr = (seq_t *)realloc(seq_cache,
                                                   new_cap * sizeof(seq_t));
                seq_cache = new_ptr;
                seq_cap = new_cap;
            }
            seq_cache[seq_len++] = cand;
        }
    }
}

/* public entry point for the sequence */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* offset starts at 1; unused */
    ensure_cache((size_t)n);
    return seq_cache[(size_t)(n - 1)];
}
