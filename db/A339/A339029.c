/*
 * A339029  Expansion of (1 + 4*x - 20*x^2 + 8*x^3 + 33*x^4 - 4*x^5 -
 *          33*x^6)/(1 - 2*x)^4.
 * https://oeis.org/A339029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-7)*(588 + 367*n + 84*n^2 + 9*n^3) for n ≥ 3 a(0) =
 *            1, a(1) = 12, a(2) = 52 The implementation avoids overflow by
 *            delaying or retarding the multiplication by powers of two
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 50, u128 n <= 111,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)52;

    seq_t sn = (seq_t)n;

    /* Compute the polynomial part: 588 + 367*n + 84*n^2 + 9*n^3 */
    seq_t term = (seq_t)588;
    term += sn * (seq_t)367;
    term += (sn * sn) * (seq_t)84;
    term += (sn * sn * sn) * (seq_t)9;

    /* Apply the power‑of‑2 factor.  We shift right if the exponent is
       negative, otherwise shift left (or multiply by pow2). */
    if (n > 7) {
        /* 2^(n-7) * term */
        return term * pow2((idx_t)(n - 7));
    } else {
        /* term / 2^(7-n) – the division is exact. */
        return term >> (7 - n);
    }
}
