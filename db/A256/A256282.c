/*
 * A256282  Expansion of f(-q^3) * psi(q^3)^3 / (psi(q) * psi(q^9)) in powers
 *          of q where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A256282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256282:  a(n) = -b(n) where b(n) is multiplicative.
   For n>0:  b(2^e) = -1 (e>0).
             b(3^e) = 2*(1+(-1)^e).
             For primes p≡1 (mod 4): b(p^e) = e+1.
             For primes p≡3 (mod 4), p≠3: b(p^e) = (1+(-1)^e)/2.
   a(0) = 1.  (All other arithmetic uses integer types only.) */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t b = 1;
    uint64_t m = (uint64_t)n;
    /* factor 2 separately */
    if (m % 2 == 0) {
        /* b(2^e) = -1, regardless of e>0 */
        b = -1;
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t f;
            if (p == 3) {
                f = (e % 2 == 0) ? (seq_t)4 : (seq_t)0;
            } else if (p % 4 == 1) {
                f = (seq_t)(e + 1);
            } else { /* p % 4 == 3, p != 3 */
                f = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
            b *= f;
            if (b == 0) {
                break;
            }
        }
    }
    if (b != 0 && m > 1) { /* remaining prime factor */
        uint64_t p = m;
        uint64_t e = 1;
        seq_t f;
        if (p == 3) {
            f = 2 * (seq_t)(1 + ((seq_t)e % 2 == 0 ? (seq_t)1 : (seq_t)-1));
            /* Actually for p==3, e=1 => f=0, e=2 => f=4 */
            if (e % 2 == 0) f = (seq_t)4;
            else f = (seq_t)0;
        } else if (p % 4 == 1) {
            f = (seq_t)(e + 1);
        } else { /* p % 4 == 3 */
            f = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
        }
        b *= f;
    }
    return -b;
}
