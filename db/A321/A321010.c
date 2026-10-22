/*
 * A321010  Numbers k such that f(k^2) = k, where f is Eric Angelini's
 *          remove-repeated-digits map x->A320486(x).
 * https://oeis.org/A321010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) is the n‑th integer k such that f(k²)=k, where * f(n)
 *            removes all digits that occur more than once in the decimal *
 *            representation of n. The function generates the sequence by *
 *            brute‑force testing successive integers
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   filter_once
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/filter_once.h"

static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t next_candidate = 0;   /* next integer k to test */

/* Grow seq_cache to hold at least 'need' elements. */
static void ensure_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap = new_cap;
    }
}

/* Generate the sequence values until it contains at least n terms. */
static void generate_up_to(idx_t n)
{
    if (seq_len == 0) {
        ensure_cap((size_t)n);
        seq_cache[0] = 0;
        seq_len = 1;
        next_candidate = 1;
    } else {
        ensure_cap((size_t)n);
    }

    while (seq_len < (size_t)n) {
        seq_t sq = next_candidate * next_candidate;
        seq_t fval = filter_once(sq);
        if (fval == next_candidate) {
            seq_cache[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1)=0 */
    generate_up_to(n);
    return seq_cache[n - 1];
}
