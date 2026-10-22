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
 * Helpers:   count_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_valid.h"

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
