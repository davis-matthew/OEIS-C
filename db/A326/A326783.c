/*
 * A326783  BII-numbers of uniform set-systems.
 * https://oeis.org/A326783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the nth uniform BII‑number */
/* a(1) = 0, a(2) = 1, a(3) = 2, ... (OEIS A326783) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* popcount for a 64‑bit integer */
static inline int popcount_uint64(idx_t x) {
    return __builtin_popcountll(x);
}

/* true iff every binary index of x has the same binary weight */
static bool is_uniform(idx_t x) {
    if (x == 0) return true;   /* 0 is uniform by definition */

    int expected = -1;
    idx_t mask = 1;
    int pos = 1;               /* positions are 1‑based, LSB is position 1 */

    while (mask <= x) {
        if (x & mask) {
            int w = popcount_uint64(pos);
            if (expected == -1)
                expected = w;
            else if (w != expected)
                return false;
        }
        mask <<= 1;
        pos++;
    }
    return true;
}

/* sequence a(n): n >= 1 */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* safety, not used for OEIS offset */

    idx_t count = 0;
    idx_t val   = 0;

    while (true) {
        if (is_uniform(val)) {
            count++;
            if (count == n)
                return (seq_t)val;
        }
        val++;
    }
}
