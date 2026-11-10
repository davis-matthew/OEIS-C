/*
 * A338243  a(0) = 0, a(n) = A338241(2*n-1) for any n > 0.
 * https://oeis.org/A338243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = A338241(2*n-1); A338241: b(0)=0, b(3m)=3*b(m),
 *            b(3m+1)=1-3*b(m), b(3m+2)=3*b(m)-1
 * Range:     u8 n <= 255, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Validated: standard, pass, 84 terms through n = 6110 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A338241_mag(idx_t n, bool *neg)
{
    int d[48];
    int len = 0;
    while (n > 0) {
        d[len++] = (int)(n % 3);
        n /= 3;
    }
    seq_t M = 0;
    bool s = false;
    for (int i = len - 1; i >= 0; i--) {
        int r = d[i];
        if (r == 0) {
            M = M * 3;
        } else if (r == 1) {
            /* 1 - 3*sub */
            if (s) {
                M = M * 3 + 1;
                s = false;
            } else if (M == 0) {
                M = 1;
            } else {
                M = M * 3 - 1;
                s = true;
            }
        } else {
            /* 3*sub - 1 */
            if (s) {
                M = M * 3 + 1;
            } else if (M == 0) {
                M = 1;
                s = true;
            } else {
                M = M * 3 - 1;
            }
        }
    }
    *neg = s;
    return M;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    bool neg;
    return A338241_mag(2 * n - 1, &neg);
}
