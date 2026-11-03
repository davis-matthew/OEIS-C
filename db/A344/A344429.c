/*
 * A344429  a(n) = Sum_{k=1..n} mu(k) * k^n.
 * https://oeis.org/A344429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * k^n
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 372 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int mu = mu_of(k);
        if (mu == 0) continue;
        seq_t pw = ipow((seq_t)k, (seq_t)n);
        sum += (seq_t)mu * pw;
    }
    return sum;
}
