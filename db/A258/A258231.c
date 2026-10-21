/*
 * A258231  Numbers n such that both n and n squared contain exactly the same
 *          digits, and n is not divisible by 10.
 * https://oeis.org/A258231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 9455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that n and n^2 contain exactly the same
   digits (as sets) and n is not divisible by 10.  The function
   performs a brute‑force scan to find the n-th term.  */
#include <stdint.h>

static inline unsigned digit_mask_u64(uint64_t x) {
    unsigned mask = 0;
    if (x == 0) {
        mask |= 1u << 0;
        return mask;
    }
    while (x) {
        mask |= 1u << (unsigned)(x % 10u);
        x /= 10u;
    }
    return mask;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* not used, offset is 1 */
    idx_t found = 0;
    uint64_t m = 1;
    for (;;) {
        if (m % 10u != 0u) {
            unsigned mask_m = digit_mask_u64(m);
            uint64_t sq = m * m;          /* fits in uint64_t for all outputs needed */
            if (digit_mask_u64(sq) == mask_m) {
                ++found;
                if (found == n)
                    return (seq_t)m;
            }
        }
        ++m;
    }
}
