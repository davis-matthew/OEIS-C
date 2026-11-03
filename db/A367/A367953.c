/*
 * A367953  Fixed point of the morphism 2 -> {2,2,4}, t -> {t-2,t,t,t+2} (for
 *          t > 2), starting from {2}.
 * https://oeis.org/A367953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24310, u16 n <= 24310, u32 n <= 24310, u64 n <= 24310,
 *            u128 n <= 24310, big any n
 * Validated: standard, pass, 106 terms through n = 9198 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by expanding the morphism until the required
   prefix length is reached.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void ensure_prefix(idx_t n)
{
    if (seq == NULL) {
        seq_cap = 1;
        seq = alloc_seq_array(seq_cap);
        seq[0] = (seq_t)2;
        seq_len = 1;
    }
    while (seq_len < (size_t)n) {
        size_t new_cap = seq_len * 4 + 1;      /* each element expands to at most 4 */
        seq_t *new_seq = alloc_seq_array(new_cap);
        size_t new_len = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t t = seq[i];
            if (t == (seq_t)2) {
                new_seq[new_len++] = (seq_t)2;
                new_seq[new_len++] = (seq_t)2;
                new_seq[new_len++] = (seq_t)4;
            } else {
                new_seq[new_len++] = t - (seq_t)2;
                new_seq[new_len++] = t;
                new_seq[new_len++] = t;
                new_seq[new_len++] = t + (seq_t)2;
            }
        }
        free_seq_array(seq);
        seq = new_seq;
        seq_cap = new_cap;
        seq_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for n < 1 */
    ensure_prefix(n);
    return seq[(size_t)n - 1];
}
