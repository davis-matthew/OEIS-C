/*
 * A253206  Coefficients of the Dirichlet series for zeta(5*s)/zeta(s).
 * https://oeis.org/A253206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e of a(p^e) where a(p^e) = 1
 *            if e ≡ 0 (mod 5), -1 if e ≡ 1 (mod 5), 0 otherwise. (Hence a(n)
 *            = 1 for n a fifth power, -1 if n = p * (fifth‑power), otherwise
 *            0; multiplicatively extended.)
 * Range:     i8 n <= 500, i16 n <= 500, i32 n <= 500, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative computation of a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; return 0 for safety */
        return 0;

    uint64_t m = n;          /* mutable copy to factor */
    seq_t sign = 1;          /* product of local factors */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            uint64_t mod = exp % 5ULL;
            if (mod == 0)
                continue;           /* factor = 1, nothing to do */
            if (mod == 1)
                sign = -sign;       /* factor = -1 */
            else
                return 0;           /* factor = 0 → overall product 0 */
        }
    }

    /* Remaining prime factor, if any, has exponent 1 */
    if (m > 1) { /* m is prime */
        /* exp = 1 → mod = 1 → factor = -1 */
        sign = -sign;
    }

    return sign;   /* 1 or -1 (or 0 from early return) */
}
