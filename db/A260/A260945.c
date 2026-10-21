/*
 * A260945  Expansion of (2*b(q^4) - b(q) - b(q^2)) / 3 in powers of q where
 *          b() is a cubic AGM theta function.
 * https://oeis.org/A260945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) defined multiplicatively by a(2^e) = -(-1)^e (e>0) a(3^e) =
 *            -2 (e>0) a(p^e) = e+1 if p ≡ 1 (mod 6) a(p^e) = (1+(-1)^e)/2 if
 *            p ≡ 5 (mod 6) a(1) = 1, a(0) = 0 for all other n, a(n) = ∏
 *            a(p^e) over its prime powers
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            seq_t term = 1;
            if (p == 2) {
                term = (exp % 2 == 0) ? (seq_t)-1 : (seq_t)1; /* -(-1)^e */
            } else if (p == 3) {
                term = -2;
            } else if (p % 6 == 1) {
                term = exp + 1; /* e+1 */
            } else if (p % 6 == 5) {
                term = (exp % 2 == 0) ? (seq_t)1 : (seq_t)0; /* (1+(-1)^e)/2 */
            }
            result *= term;
            if (result == 0) return 0;
        }
    }

    if (temp > 1) { /* remaining prime factor with exponent 1 */
        idx_t p = temp;
        seq_t term;
        if (p == 2) {
            term = (1 % 2 == 0) ? (seq_t)-1 : (seq_t)1; /* exp=1 odd -> 1 */
        } else if (p == 3) {
            term = -2;
        } else if (p % 6 == 1) {
            term = (seq_t)2;           /* e+1 with e=1 */
        } else if (p % 6 == 5) {
            term = (1 % 2 == 0) ? (seq_t)1 : (seq_t)0; /* e=1 odd ->0 */
        } else {
            term = 1;
        }
        result *= term;
    }

    return result;
}
