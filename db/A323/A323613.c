/*
 * A323613  Antidiagonal sums of A323182.
 * https://oeis.org/A323613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} U_{n-k}(k), where U_m(x) is the Chebyshev
 *            polynomial of the second kind: U_0(x)=1, U_1(x)=2x,
 *            U_m(x)=2x*U_{m-1}(x)-U_{m-2}(x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 531 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 1; k <= n; ++k) {
        idx_t m = n - k;
        seq_t u;
        if (m == 0) {
            u = 1;
        } else {
            seq_t p2 = 1;
            seq_t p1 = (seq_t)2 * (seq_t)k;
            seq_t dk2 = (seq_t)2 * (seq_t)k;
            idx_t t;
            for (t = 2; t <= m; ++t) {
                seq_t c = dk2 * p1 - p2;
                p2 = p1;
                p1 = c;
            }
            u = p1;
        }
        sum += u;
    }
    /* k = 0 term: U_n(0) = 1 if n%4==0, -1 if n%4==2, else 0 */
    if (n % 4 == 0) sum += 1;
    else if (n % 4 == 2) sum -= 1;
    return sum;
}
