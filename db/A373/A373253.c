/*
 * A373253  a(n) is -1, 0, or 1 such that a(n) == A003415(n) (mod 3), where
 *          A003415 is the arithmetic derivative.
 * https://oeis.org/A373253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the sign of A003415(n) modulo 3: a(n) = 0 if A003415(n)
 *            ≡ 0 (mod 3) 1 if A003415(n) ≡ 1 (mod 3) -1 if A003415(n) ≡ 2
 *            (mod 3)
 * Range:     i8 n <= 63, i16 n <= 9243, i32 n <= 9243, i64 n <= 9243,
 *            i128 n <= 9243, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the arithmetic derivative for n and return its value
   modulo 3 mapped to -1, 0 or 1.  The implementation uses a
   simple trial‑division factorisation for n up to about 10 000
   (the time limit easily covers this size). */

seq_t a(idx_t n)
{
    /* n = 0 or 1 → derivative 0 */
    if (n < 2)
        return (seq_t)0;

    seq_t dn = 0;            /* arithmetic derivative of n */
    const idx_t original_n = n;
    idx_t temp = n;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        int exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        dn += ((seq_t)original_n / (seq_t)2) * exp;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            dn += ((seq_t)original_n / (seq_t)p) * exp;
        }
    }

    /* if a prime remains */
    if (temp > 1) {
        dn += ((seq_t)original_n / (seq_t)temp) * (seq_t)1;
    }

    /* map modulo 3 remainder to -1,0,1 */
    int r = (int)(dn % 3);
    if (r == 0)
        return (seq_t)0;
    if (r == 1)
        return (seq_t)1;
    /* r == 2 → -1 (since 2 ≡ -1 mod 3) */
    return (seq_t)(-1);
}
