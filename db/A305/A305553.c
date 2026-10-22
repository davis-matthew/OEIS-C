/*
 * A305553  Numbers that are not the sum of 2 squares and a 4th power.
 * https://oeis.org/A305553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A305553: numbers that are not representable as a² + b² + c⁴, offset 1 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

/* Test whether x can be written as a² + b² with non‑negative integers a,b. */
static bool is_sum_two_squares(seq_t x)
{
    for (idx_t a = 0; ; ++a) {
        seq_t a2 = (seq_t)a * a;
        if (a2 > x) break;
        seq_t rem = x - a2;
        seq_t b = floor_sqrt(rem);
        if (b * b == rem) return true;
    }
    return false;
}

/* Test whether n can be written as a² + b² + c⁴. */
static bool representable(seq_t n)
{
    for (idx_t c = 0; ; ++c) {
        seq_t c2 = (seq_t)c * c;
        seq_t c4 = c2 * c2;
        if (c4 > n) break;
        seq_t rem = n - c4;
        if (is_sum_two_squares(rem)) return true;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static storage for computed terms. */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static seq_t candidate = 1;

    /* Expand the sequence until it contains at least n elements. */
    while (len < (size_t)n) {
        if (!representable(candidate)) {
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 1 : cap * 2;
                seq = seq_realloc(seq, cap, new_cap);
                cap = new_cap;
            }
            seq[len++] = candidate;
        }
        candidate++;
    }

    return seq[(size_t)n - 1];
}
