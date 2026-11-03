/*
 * A243530  Prime numbers containing the string 14.
 * https://oeis.org/A243530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime containing the decimal substring 14
 * Range:     u8 n <= 1, u16 n <= 237, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 893 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return true if v contains the decimal substring "14" */
static bool contains14(idx_t v) {
    if (v < 10) return false;
    for (; v >= 10; v /= 10) {
        if (v % 100 == 14) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t candidate = 1;; ++candidate) {
        if (!contains14(candidate)) continue;
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
