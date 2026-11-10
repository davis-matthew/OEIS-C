/*
 * A287403  Positions of 1 in A287401.
 * https://oeis.org/A287403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n - 2 + [A287401(n) != 2], where A287401(n) is the
 *            symbol reached by running base-3 digits of n-1 through 0->012,
 *            1->210, 2->120. Each block of 3 positions holds exactly one 1
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9899 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A287403_trans[3][3] = {
    {0, 1, 2},
    {2, 1, 0},
    {1, 2, 0}
};

static int A287403_sym(idx_t k)
{
    unsigned char digit[64];
    int len = 0;
    do {
        digit[len++] = (unsigned char)(k % 3);
        k /= 3;
    } while (k);
    int s = 0;
    for (int i = len - 1; i >= 0; --i) {
        s = A287403_trans[s][digit[i]];
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t k = n - 1;
    int s = A287403_sym(k);
    int off = (s == 2) ? 0 : 1;
    return (seq_t)3 * (seq_t)k + (seq_t)1 + (seq_t)off;
}
