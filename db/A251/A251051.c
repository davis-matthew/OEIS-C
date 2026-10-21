/*
 * A251051  Number of (n+1)X(4+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 18, u64 n <= 44, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) -25*a(n-2) +35*a(n-3) -7*a(n-4) -49*a(n-5)
                   +77*a(n-6) -55*a(n-7) +20*a(n-8) -3*a(n-9),  n>12
   Initial values: a(1)=2243, a(2)=11280, a(3)=24893, a(4)=59215,
                   a(5)=123396, a(6)=262200, a(7)=533327, a(8)=1107413,
                   a(9)=2297926, a(10)=4888898, a(11)=10549941,
                   a(12)=23231818 */
#include <stdint.h>

static const seq_t _base[12] = {
    (seq_t)2243ULL,
    (seq_t)11280ULL,
    (seq_t)24893ULL,
    (seq_t)59215ULL,
    (seq_t)123396ULL,
    (seq_t)262200ULL,
    (seq_t)533327ULL,
    (seq_t)1107413ULL,
    (seq_t)2297926ULL,
    (seq_t)4888898ULL,
    (seq_t)10549941ULL,
    (seq_t)23231818ULL
};

seq_t a(idx_t n) {
    if (n <= 12) {
        return _base[n-1];
    }

    static const int coef[9] = {8, -25, 35, -7, -49, 77, -55, 20, -3};
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i) {
        buf[i] = _base[i];
    }

    idx_t pos = 12 % 12;      /* next write position in the circular buffer */
    for (idx_t i = 12; i < n; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int j = 1; j <= 9; ++j) {
            idx_t idx = (pos + 12 - j) % 12;
            if (coef[j-1] > 0) {
                pos_sum += (seq_t)coef[j-1] * buf[idx];
            } else {
                neg_sum += (seq_t)(-coef[j-1]) * buf[idx];
            }
        }
        buf[pos] = pos_sum - neg_sum;   /* pos >= neg so subtraction is safe */
        pos = (pos + 1) % 12;
    }

    /* a(n) is stored in the slot just written */
    return buf[(pos + 12 - 1) % 12];
}
