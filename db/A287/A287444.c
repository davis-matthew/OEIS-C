/*
 * A287444  Positions of 0 in A287443.
 * https://oeis.org/A287444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n-2 + [A287443(n)=1]; each symbol's image has exactly
 *            one 0, at offset 1 (1-based) unless the symbol is 1, where it is
 *            at offset 2
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9969 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A287443_trans[3][3] = {
    {0, 1, 2},
    {2, 0, 1},
    {0, 2, 1}
};

static int A287443_sym(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n - 1ULL;
    unsigned int digits[64];
    int dcnt = 0;
    while (m > 0) {
        digits[dcnt++] = (unsigned int)(m % 3ULL);
        m /= 3ULL;
    }
    int s = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        s = A287443_trans[s][digits[i]];
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t r = (seq_t)3 * (seq_t)n - (seq_t)2;
    if (A287443_sym(n) == 1) r += (seq_t)1;
    return r;
}
