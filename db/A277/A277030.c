/*
 * A277030  Smallest m such that b^phi(n) == b^m (mod n) for every integer b,
 *          where phi(n) = A000010(n).
 * https://oeis.org/A277030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n==1 4 if n==8 or n==24 Carmichael(n) (A002322)
 *            otherwise Carmichael(n) = lcm(p_i^e_i-1*(p_i-1) for odd primes
 *            p_i and 1, 2, 2^(e-2) for p_i=2, e_i>2 )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 8 || n == 24) return 4;

    idx_t m = n;
    seq_t result = 1;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;

        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t comp;
        if (p == 2) {
            if (exp == 1)       comp = 1;
            else if (exp == 2)  comp = 2;
            else                comp = ((seq_t)1) << (exp - 2);   /* 2^(exp-2) */
        } else {
            /* λ(p^exp) = p^(exp-1)*(p-1)  for odd p */
            comp = 1;
            for (idx_t i = 0; i < exp - 1; ++i) comp *= (seq_t)p;
            comp *= (seq_t)(p - 1);
        }

        seq_t g = gcd(result, comp);
        result = (result / g) * comp;
    }

    if (m > 1) {   /* remaining prime factor (exponent 1) */
        seq_t comp = (m == 2) ? 1 : (seq_t)(m - 1);
        seq_t g = gcd(result, comp);
        result = (result / g) * comp;
    }

    return result;
}
