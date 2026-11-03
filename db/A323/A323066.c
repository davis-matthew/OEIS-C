/*
 * A323066  Numbers whose binary complement (A035327) is a square.
 * https://oeis.org/A323066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   bit_length, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n‑th term of OEIS A323066: numbers whose binary complement is a perfect square. (search). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    static seq_t *vals = NULL;
    static size_t cap = 0;
    static size_t cnt = 0;
    static idx_t next = 0;

    while (cnt < n) {
        seq_t cur = (seq_t)next;

        unsigned width;
        if (cur == (seq_t)0) {
            width = 1;
        } else {
            width = bit_length(cur);
        }

        seq_t mask;
        if (width == sizeof(seq_t) * 8) {
            mask = ~((seq_t)0);
        } else {
            mask = (((seq_t)1 << width) - (seq_t)1);
        }

        seq_t comp = mask - cur;

        seq_t root = floor_sqrt(comp);
        if (root * root == comp) {
            if (cnt >= cap) {
                size_t new_cap = cap ? cap << 1 : 8;
                seq_t *tmp = mem_resize(seq_t, vals, new_cap);
                vals = tmp;
                cap = new_cap;
            }
            vals[cnt++] = cur;
        }
        ++next;
    }

    return vals[n - 1];
}
