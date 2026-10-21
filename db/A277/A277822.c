/*
 * A277822  a(n) = index of the column where n is located in array A277880.
 * https://oeis.org/A277822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9543, u16 n <= 9543, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n ≥ 1,
   a(n) = 1,                          if popcount(n) is odd
   a(n) = 1 + a(floor(n/2)),          if popcount(n) is even
   (i.e. a(n) = 1 + number of successive halvings until an odious number is reached) */
#include <stdint.h>
#include <stdbool.h>

/* compute popcount for 64‑bit values */
static inline uint32_t popcount_u64(uint64_t x) {
    uint32_t c = 0;
    while (x) {
        c += (uint32_t)(x & 1U);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t steps = 0;
    uint64_t m = (uint64_t)n;
    while (true) {
        uint32_t pc = popcount_u64(m);
        if (pc & 1U) {                /* odd popcount → odious number */
            break;
        }
        m >>= 1;                       /* floor(m/2) */
        steps++;                       /* one more iteration */
    }
    return 1 + steps;
}
