/*
 * A328320  Numbers for which A328311(n) = 1 + A051903(A003415(n)) -
 *          A051903(n) is zero (including 1 as the initial term).
 * https://oeis.org/A328320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 8127, u32 n <= 8127, u64 n <= 8127,
 *            u128 n <= 8127, big any n
 * Helpers:   property_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A328320: numbers n for which 1 + A051903(A003415(n)) - A051903(n) == 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/property_n.h"

/* Cached sequence of values. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t next_test = 0;   /* number to be tested next */

/* Ensure that seq contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;          /* 1 is always in the sequence */
        seq_len = 1;
        next_test = 2;              /* start testing from 2 */
    }

    while (seq_len < (size_t)n) {
        uint64_t m = next_test++;
        if (m == 1 || property_n(m)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_terms(n);
    return seq[(size_t)n - 1];
}
