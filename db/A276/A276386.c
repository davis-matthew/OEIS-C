/*
 * A276386  Write A003511(n) in the base {1, 3, 4, 11, 15, 41, 56, 153, 209,
 *          ...} (see A002530).
 * https://oeis.org/A276386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A003511(n) written in the greedy base b_j = A002530(j+1)
 *            = {1,3,4,11,15,41,...}, digits 0..2, read as a decimal number;
 *            A003511(n) = floor(n*(1+sqrt(3))/2)
 * Range:     u8 n <= 8, u16 n <= 30, u32 n <= 570, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 964 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 a003511_isqrt3(uint64_t n) {
    unsigned __int128 x = (unsigned __int128)n * n * 3u;
    unsigned __int128 g = (unsigned __int128)n * 2u;
    if (g == 0) return 0;
    for (;;) {
        unsigned __int128 y = (g + x / g) / 2u;
        if (y >= g) break;
        g = y;
    }
    return g;
}

static seq_t a003511_isqrt3_seq(idx_t n) {
    seq_t x = (seq_t)n * (seq_t)n * 3;
    seq_t g = (seq_t)n * 2;
    for (;;) {
        seq_t y = (g + x / g) / 2;
        if (y >= g) break;
        g = y;
    }
    return g;
}

static seq_t A003511_a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n < ((idx_t)1 << 61)) {
        unsigned __int128 m = a003511_isqrt3((uint64_t)n);
        uint64_t r = (uint64_t)(((unsigned __int128)n + m) / 2u);
        return (seq_t)r;
    }
    return ((seq_t)n + a003511_isqrt3_seq(n)) / 2;
}

seq_t a(idx_t n) {
    seq_t m = A003511_a(n);
    seq_t c[300];
    c[0] = 0; c[1] = 1; c[2] = 1; c[3] = 3;
    int top = 3;
    while (c[top] <= m && top < 295) {
        top++;
        c[top] = 4 * c[top - 2] - c[top - 4];
    }
    seq_t result = 0;
    for (int i = top; i >= 2; i--) {
        seq_t d = 0;
        if (c[i] <= m) {
            d = m / c[i];
            m -= d * c[i];
        }
        result = result * 10 + d;
    }
    return result;
}
