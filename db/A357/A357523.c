/*
 * A357523  Reverse run lengths in binary expansions of terms of A166535: for
 *          n > 0, a(n) is the unique k such that A166535(k) =
 *          A056539(A166535(n)); a(0) = 0.
 * https://oeis.org/A357523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A166535(k) = A056539(A166535(n)), a(0) = 0;
 *            computed by building the list A166535 incrementally and ranking
 *            via binary search
 * Range:     u8 n <= 177, u16 n <= 12639, u32 n <= 12639, u64 n <= 12639,
 *            u128 n <= 12639, big any n
 * Validated: standard, pass, 84 terms through n = 9872 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A357523_cache = NULL;
static idx_t A357523_size = 0;
static idx_t A357523_cap = 0;

static bool A357523_passes(seq_t x)
{
    int prev = -1, run = 0;
    seq_t y = x;
    while (y > 0) {
        int bit = (int)(y & 1);
        if (bit == prev) ++run; else { run = 1; prev = bit; }
        if (run > 3) return false;
        y >>= 1;
    }
    return true;
}

static void A357523_push(seq_t v)
{
    if (A357523_size >= A357523_cap) {
        idx_t nc = A357523_cap == 0 ? 64 : A357523_cap * 2;
        seq_t *t = mem_resize(seq_t, A357523_cache, nc);
        if (!t) abort();
        A357523_cache = t;
        A357523_cap = nc;
    }
    A357523_cache[A357523_size++] = v;
}

static seq_t A357523_next_cand = 1;

static void A357523_extend_count(idx_t n)
{
    while (A357523_size < n) {
        if (A357523_passes(A357523_next_cand)) A357523_push(A357523_next_cand);
        ++A357523_next_cand;
    }
}

static void A357523_extend_value(seq_t w)
{
    while (A357523_size == 0 || A357523_cache[A357523_size - 1] < w) {
        if (A357523_passes(A357523_next_cand)) A357523_push(A357523_next_cand);
        ++A357523_next_cand;
    }
}

/* A056539 */
static seq_t A357523_rev(seq_t v)
{
    int len = 0;
    seq_t t = v;
    while (t > 0) { ++len; t >>= 1; }
    int runs = 0, prev = -1;
    seq_t rev = 0;
    t = v;
    for (int i = 0; i < len; ++i) {
        int bit = (int)(t & 1);
        if (bit != prev) { ++runs; prev = bit; }
        rev = (rev << 1) | (seq_t)bit;
        t >>= 1;
    }
    if ((runs & 1) == 0) {
        seq_t mask = (((seq_t)1) << len) - 1;
        rev ^= mask;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A357523_extend_count(n);
    seq_t v = A357523_cache[n - 1];
    seq_t w = A357523_rev(v);
    A357523_extend_value(w);
    idx_t lo = 0, hi = A357523_size;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (A357523_cache[mid] < w) lo = mid + 1;
        else hi = mid;
    }
    return (seq_t)(lo + 1);
}
