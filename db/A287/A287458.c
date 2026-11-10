/*
 * A287458  Positions of 2 in A287455.
 * https://oeis.org/A287458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n if A287455(n) = 0, else 3n-2 (the 2 in the image of
 *            the n-th symbol under 0->012, 1->201, 2->210)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9593 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned char A287458_sym(idx_t n)
{
    if (n == 0) return 0;
    idx_t idx = n - 1;
    unsigned char digits[64];
    int len = 0;
    while (idx > 0) {
        digits[len++] = (unsigned char)(idx % 3);
        idx /= 3;
    }
    static const unsigned char sg[3][3] = {
        {0, 1, 2},
        {2, 0, 1},
        {2, 1, 0}
    };
    unsigned char sym = 0;
    for (int i = len - 1; i >= 0; --i) {
        sym = sg[sym][digits[i]];
    }
    return sym;
}

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n * 3;
    if (A287458_sym(n) != 0) r -= 2;
    return r;
}
