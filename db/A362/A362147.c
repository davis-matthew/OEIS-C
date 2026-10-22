/*
 * A362147  Numbers that are not cubefull.
 * https://oeis.org/A362147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_not_cubefull_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer that is NOT cubefull
   (a number that has at least one prime factor with exponent 1 or 2). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_not_cubefull_uint64.h"

/* REQUIRED: external linkage, no other definitions. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety for offset 1 */

    idx_t count = 0;
    uint64_t val = 2;            /* numbers start at 2 */

    for (;;) {
        if (is_not_cubefull_uint64(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
        ++val;
    }
}
