/*
 * A289034  Fixed point of the morphism 0->010, 1->10 starting with 1.
 * https://oeis.org/A289034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8502 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth symbol of the fixed point of the morphism
   0 → 010, 1 → 10 starting with 1. */
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *w = NULL;
static idx_t w_len = 0;
static size_t w_cap = 0;

static void make_base(void) {
    if (!w) {
        w = mem_new(seq_t, 1);
        w[0] = 1;
        w_len = 1;
        w_cap = 1;
    }
}

static void grow_to(idx_t target) {
    make_base();
    while (w_len < target) {
        idx_t old_len = w_len;
        size_t new_cap = (size_t)old_len * 3;
        seq_t *next = mem_new(seq_t, new_cap);
        idx_t pos = 0;
        for (idx_t i = 0; i < old_len; ++i) {
            if (w[i] == 1) {              /* 1 → 10 */
                next[pos++] = 1;
                next[pos++] = 0;
            } else {                      /* 0 → 010 */
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            }
        }
        mem_free(w);
        w = next;
        w_len = pos;
        w_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, should not be called */
    grow_to(n);
    return w[n - 1];
}
