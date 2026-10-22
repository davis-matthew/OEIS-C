/*
 * A333355  Number of bits in binary expansion of n minus the number of digits
 *          of n when written in base 3.
 * https://oeis.org/A333355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(log2(n)) - floor(log3(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

/* compute floor(log3(n)) for n >= 1 using 64‑bit arithmetic */
static inline int _floor_log3_uint64(uint64_t n) {
    uint64_t p = 1;
    int l = 0;
    /* multiply by 3 while the result stays <= n/3 to avoid overflow */
    while (p <= n / 3) {
        p *= 3;
        ++l;
    }
    return l;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* offset starts at 1, behavior below not used */
    seq_t log2 = (seq_t)floor_log2_uint64(n);
    seq_t log3 = (seq_t)_floor_log3_uint64(n);
    return log2 - log3;
}
