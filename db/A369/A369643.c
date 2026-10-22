/*
 * A369643  a(n) = 1 if n' / gcd(n,n') is a multiple of 3, otherwise 0. Here
 *          n' stands for the arithmetic derivative of n, A003415.
 * https://oeis.org/A369643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the reduced arithmetic derivative n′/gcd(n,n′) is a
 *            multiple of 3, otherwise a(n) = 0. The arithmetic derivative is
 *            n′ = Σ e·(n/p) over all distinct prime factors p^e of n
 * Range:     u8 n <= 338, u16 n <= 8472, u32 n <= 8472, u64 n <= 8472,
 *            u128 n <= 8472, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) should be 1.  The calculation below
       works for all n ≥ 1; for n = 0 we return 0 (undefined). */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;       /* the original n as seq_t               */
    seq_t deriv = 0;           /* accumulator for the arithmetic derivative */

    uint64_t temp = n;         /* temporary copy for factorisation      */
    uint64_t p;                /* current prime divisor                  */
    uint32_t exp;              /* exponent of the current prime          */

    /* factor n by trial division; this is acceptable for the dynamic
       range required by the test harness (up to about 10⁴). */
    if (temp % 2 == 0) {
        exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        deriv += ((seq_t)exp * (nn / (seq_t)2));
    }

    for (p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            deriv += ((seq_t)exp * (nn / (seq_t)p));
        }
    }

    if (temp > 1) {                   /* remaining prime factor */
        deriv += (nn / (seq_t)temp);  /* exponent is 1         */
    }

    /* reduce the derivative by its gcd with n and test divisibility by 3 */
    seq_t g  = gcd(nn, deriv);
    seq_t t  = deriv / g;
    return (t % (seq_t)3 == 0) ? (seq_t)1 : (seq_t)0;
}
