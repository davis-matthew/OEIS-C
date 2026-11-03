/*
 * A305634  Even numbers that are not perfect powers.
 * https://oeis.org/A305634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = even numbers that are NOT perfect powers (m^k, k>=2)
 * Range:     u8 n <= 26, u16 n <= 968, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9744 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static bool is_perfect_power(seq_t x)
{
    /* A perfect power m^k with k>=2.  Only need to test for even x. */
    if (x < 4) return false;      /* 2,3 are not perfect powers with k>=2 */

    /* Limit for base m: floor(sqrt(x)) */
    seq_t limit = floor_sqrt(x);

    for (idx_t m = 2; m <= (idx_t)limit; ++m) {
        seq_t base = (seq_t)m;
        seq_t power = seq_mul(base, base);   /* m^2 */
        if (power == x) return true;
        while (power < x) {                   /* grow exponent */
            power = seq_mul(power, base);     /* power *= m */
            if (power == x) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but return 0 for safety */

    seq_t candidate = 2;       /* first even number */
    idx_t count = 0;

    for (;;) {
        if (!is_perfect_power(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        candidate += 2;        /* next even number */
    }
}
