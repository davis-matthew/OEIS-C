/*
 * A319803  a(n) = A319651(n) + A038574(n).
 * https://oeis.org/A319803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = A319651(n) + A038574(n); with c0,c1,c2 the counts of
 *            ternary digits 0,1,2 of n, A319651 = digits 2^c2 1^c1 0^c0 read
 *            in base 3, A038574 = digits 0^c0 1^c1 2^c2 read in base 3
 * Range:     u8 n <= 97, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Validated: standard, pass, 90 terms through n = 6513 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A319803_sum(idx_t n)
{
    idx_t c[3] = {0, 0, 0};
    if (n == 0) {
        c[0] = 1;
    } else {
        idx_t t = n;
        while (t != 0) {
            c[t % 3]++;
            t /= 3;
        }
    }

    /* largest arrangement: 2s, 1s, 0s */
    seq_t big = 0;
    for (int d = 2; d >= 0; --d)
        for (idx_t i = 0; i < c[d]; ++i)
            big = big * 3 + (seq_t)d;

    /* smallest arrangement: 0s, 1s, 2s */
    seq_t small = 0;
    for (int d = 0; d <= 2; ++d)
        for (idx_t i = 0; i < c[d]; ++i)
            small = small * 3 + (seq_t)d;

    return big + small;
}

seq_t a(idx_t n)
{
    return A319803_sum(n);
}
