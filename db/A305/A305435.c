/*
 * A305435  Number of proper divisors of n of the form 2^k + 1 for k >= 0.
 * https://oeis.org/A305435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors of n of the form 2^k + 1 (k ≥
 *            0)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9455 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_two_plus_one(idx_t d) {
    idx_t x = d - 1;
    return (x != 0) && ((x & (x - 1)) == 0);
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t count = 0;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 < n && is_two_plus_one(d1)) ++count;
            idx_t d2 = n / i;
            if (d2 != d1 && d2 < n && is_two_plus_one(d2)) ++count;
        }
    }
    return count;
}
