/*
 * A280054  Index of first occurrence of n in A280053, the nachos numbers
 *          based on squares.
 * https://oeis.org/A280054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q(j) + a(n-1), a(0) = 0, where j = max(1,
 *            floor(sqrt(a(n-1)))) and q(j) = j*(j+1)*(2*j+1)/6 (A000330);
 *            this is a direct iteration, not a search
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 24 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A280054_isqrt(seq_t v)
{
    if (v == 0) return 0;
    seq_t x = v / 2 + 1;
    while (1) {
        seq_t y = (x + v / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

static seq_t A280054_q(seq_t j)
{
    seq_t t = j * (j + 1) / 2;
    return t * (2 * j + 1) / 3;
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    for (idx_t k = 1; k <= n; k++) {
        seq_t j = A280054_isqrt(r);
        if (j < 1) j = 1;
        r = A280054_q(j) + r;
    }
    return r;
}
