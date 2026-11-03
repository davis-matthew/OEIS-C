/*
 * A378195  Number of 2-colorings of length n without an arithmetic
 *          progression of length 3.
 * https://oeis.org/A378195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with no
 *            monochromatic three‑term arithmetic progression
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t count_valid(idx_t len) {
    seq_t total = 0;
    uint64_t masks = (uint64_t)1 << (int)len;   /* len <= 9 here */
    for (uint64_t mask = 0; mask < masks; ++mask) {
        bool good = true;
        for (int d = 1; d <= (int)len - 2; ++d) {
            for (int i = 0; i + 2 * d < (int)len; ++i) {
                uint64_t b1 = (mask >> i) & 1ULL;
                uint64_t b2 = (mask >> (i + d)) & 1ULL;
                uint64_t b3 = (mask >> (i + 2 * d)) & 1ULL;
                if (b1 == b2 && b2 == b3) {
                    good = false;
                    goto next_mask;
                }
            }
        }
next_mask:
        if (good) ++total;
    }
    return total;
}

static seq_t cache[10];
static bool ready[10] = {0};

seq_t a(idx_t n) {
    if (n > 9) return 0;
    if (!ready[n]) {
        cache[n] = count_valid(n);
        ready[n] = true;
    }
    return cache[n];
}
