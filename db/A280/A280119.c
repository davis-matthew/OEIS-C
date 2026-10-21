/*
 * A280119  Number of nX3 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly one element, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence a(n) =
 *            6*a(n-1)+a(n-2)-52*a(n-3)+51*a(n-4)+78*a(n-5)-173*a(n-6)
 *            +136*a(n-7)+17*a(n-8)-38*a(n-9)-49*a(n-10)+32*a(n-11)
 *            -24*a(n-12)-32*a(n-13)-16*a(n-14) for n>19 initial values
 *            a(1)=2, a(2)=8, a(3)=37, a(4)=168, a(5)=705, a(6)=2852,
 *            a(7)=11249, a(8)=44120, a(9)=172216, a(10)=6696...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[20] = {
        0ULL,                /* dummy for 0 index */
        2ULL, 8ULL, 37ULL, 168ULL, 705ULL, 2852ULL,
        11249ULL, 44120ULL, 172216ULL, 669642ULL, 2593331ULL,
        10007214ULL, 38486137ULL, 147567972ULL, 564271722ULL,
        2152340508ULL, 8191329673ULL, 31110463544ULL, 117934684593ULL
    };

    if (n <= 19) {
        return init[n];
    }

    /* sliding window holding a(current-14)...a(current-1) */
    seq_t last[14];
    for (idx_t i = 0; i < 14; ++i) {
        last[i] = init[6 + i];   /* for current = 20, a(6)..a(19) */
    }

    seq_t next;
    for (idx_t cur = 20; cur <= n; ++cur) {
        seq_t pos = 6 * last[13] + last[12] + 51 * last[10]
                    + 78 * last[9] + 136 * last[7] + 17 * last[6]
                    + 32 * last[3];
        seq_t neg = 52 * last[11] + 173 * last[8] + 38 * last[5]
                    + 49 * last[4] + 24 * last[2] + 32 * last[1]
                    + 16 * last[0];
        next = pos - neg;

        if (cur == n) {
            return next;
        }

        /* shift window and append new value */
        for (idx_t i = 0; i < 13; ++i) {
            last[i] = last[i + 1];
        }
        last[13] = next;
    }

    /* Should never reach here */
    return 0;
}
