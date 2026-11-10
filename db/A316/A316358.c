/*
 * A316358  Partial sums of A316317.
 * https://oeis.org/A316358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A316317(k); A316317(0)=1, A316317(n+12) =
 *            A316317(n) + 40 for n>=1, with period-12 base values
 *            3,6,11,14,15,20,25,26,29,34,37,40
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int A316358_base[13] = {0, 3, 6, 11, 14, 15, 20, 25, 26, 29, 34, 37, 40};

seq_t a(idx_t n)
{
    seq_t Q = (seq_t)(n / 12);
    int R = (int)(n % 12);
    seq_t res = (seq_t)1;
    if (Q > 0) {
        res += Q * (seq_t)260 + (seq_t)240 * Q * (Q - 1);
    }
    for (int i = 1; i <= R; i++) {
        res += (seq_t)A316358_base[i] + (seq_t)40 * Q;
    }
    return res;
}
