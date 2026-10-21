/*
 * A285738  Greatest prime less than 2*n^2 for n > 1, a(1) = 1.
 * https://oeis.org/A285738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime less than 2*n^2 for n>1, a(1)=1
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    uint64_t target = 2ULL * (uint64_t)n * (uint64_t)n;
    uint64_t cand = target - 1;

    /* Make cand odd, except when it might be 2. */
    if (cand % 2 == 0) {
        if (cand == 2) return (seq_t)2;
        cand--;
    }

    for (uint64_t x = cand; x >= 2; ) {
        if (is_prime((idx_t)x)) return (seq_t)x;
        if (x <= 3) break;
        x -= 2;
    }

    /* Falls back to 2 if no prime found (should not occur for n>1). */
    return (seq_t)2;
}
