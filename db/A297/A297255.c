/*
 * A297255  Numbers whose base-5 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297255: numbers whose base‑5 digits have greater down‑variation than up‑variation.
   Formula: a(n) = n‑th smallest integer k>0 such that DV(k,5) > UV(k,5),
   where DV and UV are the down‑ and up‑variations of k in base 5. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static bool satisfies(seq_t v) {
    uint8_t digits[64];
    size_t len = 0;
    seq_t tmp = v;
    if (tmp == 0) return false;          /* zero is not considered */
    while (tmp != 0) {
        seq_t r = tmp % 5;
        digits[len++] = (uint8_t)r;
        tmp = tmp / 5;
    }
    seq_t DV = 0;
    seq_t UV = 0;
    for (size_t i = len; i-- > 1;) {     /* iterate from most significant to least */
        seq_t prev = digits[i];
        seq_t curr = digits[i - 1];
        if (prev > curr) {
            DV += prev - curr;
        } else if (prev < curr) {
            UV += curr - prev;
        }
    }
    return DV > UV;
}

static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t next_candidate = 1;

static void seq_cache_grow(size_t new_cap) {
    seq_t *newp = seq_alloc_new_array(new_cap);
    if (!newp) abort();
    for (size_t i = 0; i < seq_len; ++i) {
        newp[i] = seq_cache[i];
    }
    if (seq_cache) seq_free_array(seq_cache);
    seq_cache = newp;
    seq_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n=0, return 0 safely */
    if ((size_t)n <= seq_len) return seq_cache[n - 1];
    while (seq_len < (size_t)n) {
        if (satisfies(next_candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache_grow(new_cap);
            }
            seq_cache[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
