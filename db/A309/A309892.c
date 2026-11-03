/*
 * A309892  a(0) = 0, a(1) = 1, and for any n > 1, a(n) is the number of
 *          iterations of the map x -> x - gpf(x) (where gpf(x) denotes the
 *          greatest prime factor of x) required to reach 0 starting from n.
 * https://oeis.org/A309892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> x - gpf(x) to reach 0
 *            starting from n. a(0) = 0, a(1) = 1
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9968 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;            /* a(0)=0, a(1)=1 */
    }

    seq_t cnt = 0;
    uint64_t x = (uint64_t)n;

    while (x > 0) {
        uint64_t p = largest_prime_factor(x);       /* greatest prime factor of x */
        x -= p;
        cnt++;
    }

    return cnt;
}
