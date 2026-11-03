/*
 * A353518  a(n) = 1 if n is a product of superprimorials (A006939), otherwise
 *          0.
 * https://oeis.org/A353518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a product of superprimorials (A006939),
 *            otherwise 0. A superprimorial is a number whose prime
 *            factorization contains only 2 and 3, with exponent of 2 strictly
 *            greater than exponent of 3, and that exponent of 3 (if present)
 *            is odd. The empty product (n = 1) counts as 1
 * Range:     u8 n <= 103680, u16 n <= 103680, u32 n <= 103680,
 *            u64 n <= 103680, u128 n <= 103680, big any n
 * Validated: standard, pass, 144 terms through n = 9599 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A353518: 1 if n is a product of superprimorials S_k = p_1# p_2# ... p_k#, the
 * products of the first k primorials (A006939: 1, 2, 12, 360, 75600, ...),
 * otherwise 0. Searched directly: n is such a
 * product iff n = 1 or n = S_k m for some S_k > 1 dividing n with m again a
 * product of superprimorials no larger than S_k. (The original tested a
 * condition on the exponents of 2 and 3 that is not this one.) */

#include <stdint.h>

static uint64_t S[16];
static int nS = 0;

static int product_of(uint64_t n, int top)        /* using S[1..top] */
{
    if (n == 1) return 1;
    for (int k = top; k >= 1; --k)
        if (n % S[k] == 0 && product_of(n / S[k], k)) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (!nS) {
        /* S[k] = 2, 12, 360, 75600, ... while it fits */
        static const uint64_t P[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
        uint64_t f = 1, s = 1;                     /* primorial, superprimorial */
        for (int i = 0; i < 15; ++i) {
            if (f > UINT64_MAX / P[i]) break;
            f *= P[i];
            if (s > UINT64_MAX / f) break;
            s *= f;
            S[++nS] = s;
        }
    }
    if (n < 1) return (seq_t)0;
    return (seq_t)product_of((uint64_t)n, nS);
}
