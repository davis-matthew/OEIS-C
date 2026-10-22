/*
 * A318608  Moebius function mu(n) defined for the Gaussian integers.
 * https://oeis.org/A318608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 0 if n is even or has a squared prime factor; otherwise
 *            Product_{p|n} (2 - (p mod 4)) (i.e. a(n) = 0 for even n, a(n) =
 *            1 if all odd prime factors are ≡1 mod 4, a(n) = –1 if an odd
 *            number of them are ≡3 mod 4)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the Gaussian‑integer Möbius function for n ≥ 1.
   The implementation uses trial division up to √n, which is
   sufficiently fast for the usual test range (up to ~10 000). */
seq_t a(idx_t n)
{
    /* n must be ≥ 1; the caller guarantees that. */
    if ((n & 1ULL) == 0ULL)       /* even numbers → 0 */
        return (seq_t)0;

    uint64_t m = n;
    seq_t  sign = (seq_t)1;       /* multiplicative sign */

    /* Factor out odd primes. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 1)            /* squared factor detected */
                    return (seq_t)0;
            }
            /* p is an odd prime factor.  Its contribution is
               2 - (p mod 4): 1 for p≡1 (mod 4), –1 for p≡3 (mod 4). */
            if ((p & 3ULL) == 3ULL)    /* p ≡ 3 (mod 4) */
                sign = -sign;
        }
    }

    /* If a prime > sqrt(n) remains, it occurs to the first power. */
    if (m > 1) {
        if ((m & 3ULL) == 3ULL)        /* remaining prime ≡ 3 (mod 4) */
            sign = -sign;
    }

    return sign;        /* ≠0 only for square‑free odd n */
}
