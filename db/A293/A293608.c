/*
 * A293608  a(n) = (3*n + 7)*Pochhammer(n, 5) / 4!.
 * https://oeis.org/A293608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 7)*n*(n+1)*(n+2)*(n+3)*(n+4)/24
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Pochhammer n,5 = n*(n+1)*(n+2)*(n+3)*(n+4) */
    seq_t f0 = (seq_t)n;
    seq_t f1 = (seq_t)(n + 1);
    seq_t f2 = (seq_t)(n + 2);
    seq_t f3 = (seq_t)(n + 3);
    seq_t f4 = (seq_t)(n + 4);
    seq_t f5 = (seq_t)(3 * n + 7);

    seq_t num[6] = {f0, f1, f2, f3, f4, f5};
    seq_t denom  = 24;

    for (int i = 0; i < 6 && denom != 1; ++i) {
        seq_t g = gcd(num[i], denom);
        while (g > 1) {
            num[i] /= g;
            denom  /= g;
            if (denom == 1) break;
            g = gcd(num[i], denom);
        }
    }

    seq_t result = 1;
    for (int i = 0; i < 6; ++i) {
        result *= num[i];
    }
    return result;
}
