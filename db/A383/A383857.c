/*
 * A383857  Number of permutations of [n] such that precisely one rising or
 *          falling succession occurs, but without either n(n-1) or (n-1)n.
 * https://oeis.org/A383857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002464(n+1) - (n-1)*A002464(n)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A002464_a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    if (n == 2 || n == 3) return 0;

    seq_t vals[4] = {1, 1, 0, 0};

    for (idx_t i = 4; i <= n; i++) {
        seq_t coeff1 = (seq_t)(i + 1);
        seq_t coeff2 = (seq_t)(i - 2);
        seq_t coeff4 = (seq_t)(i - 3);

        seq_t ai = coeff1 * vals[3];
        ai -= coeff2 * vals[2];

        if (i >= 5) {
            seq_t coeff3 = (seq_t)(i - 5);
            ai -= coeff3 * vals[1];
        } else {
            ai += vals[1];
        }

        ai += coeff4 * vals[0];

        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = ai;
    }

    return vals[3];
}

seq_t a(idx_t n) {
    seq_t hi = A002464_a(n + 1);
    seq_t lo = A002464_a(n);
    return hi - (seq_t)(n - 1) * lo;
}
