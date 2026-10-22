/*
 * A373238  Relative of Hofstadter Q-sequence: a(n) = max(0, n+200) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) with a(n) =
 *            max(0, n+200) for n <= 0. Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 23, u16 n <= 220, u32 n <= 220, u64 n <= 220,
 *            u128 n <= 220, big any n
 * Helpers:   a_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a_signed.h"

seq_t a(idx_t n) {
    /* Forward wrapper: n is unsigned but we pass through signed helper */
    return a_signed((int64_t)n);
}
