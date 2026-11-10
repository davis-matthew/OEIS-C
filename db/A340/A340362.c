/*
 * A340362  a(n) = A005940(n) - A324106(n), where A324106(n) is multiplicative
 *          with a(p^e) = A005940(p^e).
 * https://oeis.org/A340362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A005940(n) - A324106(n), where A324106 is multiplicative
 *            with A324106(p^e) = A005940(p^e)
 * Range:     i8 n <= 44, i16 n <= 440, i32 n <= 8176, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 9869 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A340362_nth_prime(uint64_t k) {
    uint64_t c = 0, p = 1;
    while (c < k) {
        p++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= p; d++) {
            if (p % d == 0) { ok = false; break; }
        }
        if (ok) c++;
    }
    return p;
}

static seq_t A340362_doudna(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 1;
    uint64_t i = 1;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)A340362_nth_prime(i);
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t mult = 1;
    idx_t r = n;
    for (idx_t d = 2; d <= r / d; d++) {
        if (r % d == 0) {
            idx_t pe = 1;
            while (r % d == 0) { r /= d; pe *= d; }
            mult *= A340362_doudna(pe);
        }
    }
    if (r > 1) mult *= A340362_doudna(r);
    return A340362_doudna(n) - mult;
}
