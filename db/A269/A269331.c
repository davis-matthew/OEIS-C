/*
 * A269331  Indices k such that A107435(k) = A268057(k).
 * https://oeis.org/A269331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = n-th index m (reading the triangle by rows, m
 *            -> (r,k) with 1<=k<=r) such that the Euclidean algorithm length
 *            on (r,k) (A107435) equals the number of iterations of k -> r mod
 *            k to reach 0 (A268057)
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8620 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Number of division steps in the Euclidean algorithm on (r,k). */
static uint64_t A269331_euclid_len(uint64_t x, uint64_t y)
{
    uint64_t steps = 0;
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
        ++steps;
    }
    return steps;
}

/* Number of iterations of k -> r mod k until 0. */
static uint64_t A269331_iter_len(uint64_t r, uint64_t k)
{
    uint64_t cnt = 0;
    while (k != 0) {
        k = r % k;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t pos = 0;
    uint64_t r, k;

    for (r = 1;; ++r) {
        for (k = 1; k <= r; ++k) {
            ++pos;
            if (A269331_euclid_len(r, k) == A269331_iter_len(r, k)) {
                ++found;
                if (found == n)
                    return (seq_t)pos;
            }
        }
    }
}
