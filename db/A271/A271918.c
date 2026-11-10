/*
 * A271918  Erroneous version of A003585.
 * https://oeis.org/A271918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A003585(n) for n != 3, a(3) = 23 (the published misprint
 *            of 21); A003585 is found by searching odd numbers not of the
 *            form x^2 + y^2 + 10*z^2
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A003585_isqrt_local(seq_t x) {
    if (x == 0) return 0;
    if (x == 1) return 1;
    seq_t s = x;
    while (1) {
        seq_t t = (s + x / s) / 2;
        if (t >= s) return s;
        s = t;
    }
}

static bool A003585_can_represent(seq_t n) {
    seq_t z_sq = 0;
    for (seq_t z = 0; ; z++) {
        seq_t contrib = (seq_t)10 * z_sq;
        if (contrib > n) break;
        seq_t remainder = n - contrib;
        for (seq_t x = 0; ; x++) {
            seq_t x_sq = x * x;
            if (x_sq > remainder) break;
            seq_t y_sq = remainder - x_sq;
            seq_t y = A003585_isqrt_local(y_sq);
            if (y * y == y_sq) return true;
        }
        z_sq = z_sq + (seq_t)2 * z + (seq_t)1;
    }
    return false;
}

static seq_t A003585_a(idx_t n) {
    idx_t count = 0;
    seq_t num = 1;
    for (;;) {
        if (!A003585_can_represent(num)) {
            count++;
            if (count == n) return num;
        }
        num += 2;
    }
}

seq_t a(idx_t n) {
    /* The erroneous published version lists 23 in place of 21. */
    if (n == 3) return (seq_t)23;
    return A003585_a(n);
}
