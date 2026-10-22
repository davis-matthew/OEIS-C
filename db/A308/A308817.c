/*
 * A308817  Total number of parts in all m-color dihedral compositions of n
 *          (that is, the total number of parts in all dihedral compositions
 *          of n where a part of size m may be colored with one of m colors).
 * https://oeis.org/A308817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/10)*(11*F(n)+7*F(n‑1)) +
 *            (-1)^n*(n/10)*(-4*F(n)+7*F(n‑1)) - (1/5)*(5*F(n+1)+3*F(n)) -
 *            (-1)^n*(1/5)*(3*F(n)-5*F(n‑1)) + (1/2)*Σ_{s|n} φ(n/s)*F(2s)
 *            where F is Fibonacci and φ is Euler’s totient
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   _floor_isqrt_u64, diff_abs, diff_sign, fib, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/phi.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/diff_abs.h"
#include "../utils/diff_sign.h"

seq_t a(idx_t n) {
    /* Handle n = 0 or negative indices gracefully – undefined in OEIS */
    if (n == 0) return 0;

    seq_t Fn  = fib(n);          /* F(n)   */
    seq_t Fnm1 = (n > 1) ? fib(n - 1) : 0;  /* F(n-1) */
    seq_t Fn1 = fib(n + 1);      /* F(n+1) */

    idx_t parity = (n & 1) ? -1 : 1;   /* (-1)^n : +1 if n even, -1 if n odd */

    seq_t pos = 0;   /* accumulates positive contributions */
    seq_t neg = 0;   /* accumulates negative contributions */

    /* Term 1: (n/10)*(11*F(n)+7*F(n-1)) */
    seq_t term1 = (seq_t)n * ((seq_t)11 * Fn + (seq_t)7 * Fnm1);
    pos += term1;

    /* Term 2: (-1)^n*(n/10)*(-4*F(n)+7*F(n-1))  */
    seq_t t2_a = (seq_t)7 * Fnm1;   /* 7*F(n-1) */
    seq_t t2_b = (seq_t)4 * Fn;      /* 4*F(n)   */
    int  t2_inner_sign = diff_sign(t2_a, t2_b);          /* +1 if a>=b else -1 */
    seq_t t2_inner_mag = diff_abs(t2_a, t2_b);           /* |a-b|      */
    seq_t t2_mag = (seq_t)n * t2_inner_mag;              /* n*|...|    */
    int  t2_sign = parity * t2_inner_sign;                /* sign of term2 */
    if (t2_sign >= 0) pos += t2_mag;
    else              neg += t2_mag;

    /* Term 3: -(1/5)*(5*F(n+1)+3*F(n))                        */
    seq_t term3 = 2 * ((seq_t)5 * Fn1 + (seq_t)3 * Fn);     /* multiply by 2 to get denominator 10 */
    neg += term3;   /* always negative */

    /* Term 4: -(-1)^n*(1/5)*(3*F(n)-5*F(n-1))                */
    seq_t t4_a = (seq_t)3 * Fn;      /* 3*F(n)   */
    seq_t t4_b = (seq_t)5 * Fnm1;    /* 5*F(n-1) */
    int  t4_inner_sign = diff_sign(t4_a, t4_b);          /* +1 if a>=b else -1 */
    seq_t t4_inner_mag = diff_abs(t4_a, t4_b);           /* |a-b|      */
    seq_t t4_mag = 2 * t4_inner_mag;                      /* multiply by 2 for denominator 10 */
    int  t4_sign = -parity * t4_inner_sign;              /* sign of term4 */
    if (t4_sign >= 0) pos += t4_mag;
    else              neg += t4_mag;

    /* Term 5: (1/2)*Σ_{s|n} φ(n/s)*F(2s)  → multiply by 5 for common denom 10 */
    idx_t limit = _floor_isqrt_u64(n);
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t s1 = d;
            idx_t s2 = n / d;
            seq_t phi_s2 = phi(s2);                    /* φ(n/s1) */
            seq_t f2s1 = fib(2 * s1);
            pos += phi_s2 * f2s1 * (seq_t)5;           /* 5*φ*F(2s) */

            if (s1 != s2) {                            /* counterpart divisor */
                seq_t phi_s1 = phi(s1);                 /* φ(n/s2) = φ(d) */
                seq_t f2s2 = fib(2 * s2);
                pos += phi_s1 * f2s2 * (seq_t)5;        /* 5*φ*F(2s) */
            }
        }
    }

    /* Final result: (pos - neg) / 10  */
    seq_t numerator = pos;
    numerator -= neg;   /* pos ≥ neg for all n ≥ 1 */
    return numerator / (seq_t)10;
}
