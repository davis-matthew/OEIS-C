/*
 * A391432  Complement of the union of A005652(k), k>=2 and A035508(k), k>=1.
 * https://oeis.org/A391432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive m such that m = 1, or m is neither in
 *            A005652 (i.e. m + floor(sqrt(5*m^2)) is even) nor of the form
 *            Fibonacci(2k+2)-1 with k>=1; computed by a search
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u128.h"

static bool c391432_in_a005652(uint64_t k) {
    unsigned __int128 v = (unsigned __int128)k * k * 5;
    uint64_t r = (uint64_t)isqrt_u128(v);
    return ((k + r) & 1) == 1;
}

static bool c391432_in_a035508(uint64_t m) {
    /* m+1 == F(2j+2), j>=1, i.e. F(4), F(6), ... */
    uint64_t t = m + 1;
    uint64_t f1 = 1, f2 = 1; /* F(1), F(2) */
    uint64_t idx = 2;
    while (f2 < t) {
        uint64_t f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
        idx++;
        if (f2 < f1) return false;
    }
    return f2 == t && idx >= 4 && (idx % 2 == 0);
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t m = 0;
    while (count < n) {
        m++;
        if (m == 1 || (!c391432_in_a005652(m) && !c391432_in_a035508(m))) {
            count++;
        }
    }
    return (seq_t)m;
}
