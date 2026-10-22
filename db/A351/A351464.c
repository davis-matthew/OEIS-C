/*
 * A351464  Let f be multiplicative with f(prime(k)^e) = k + e*i for any k, e
 *          > 0 (where i denotes the imaginary unit); a(n) is the real part of
 *          f(n). See A351465 for the imaginary part.
 * https://oeis.org/A351464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Re(∏_{p^e || n} (π(p) + e·i))
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the real part of the multiplicative function f
   defined in OEIS A351464: f(p_k^e) = k + e·i, where k = π(p).           */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* f(1) = 1, real part 1 */

    /* factorise n by trial division */
    uint64_t m = (uint64_t)n;          /* temporary product for division */
    seq_t real = 1, imag = 0;          /* accumulated complex product */

    /* handle factor 2 separately */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t k = prime_pi(2ULL);   /* π(2) = 1 */
        seq_t e_seq = (seq_t)e;
        seq_t new_real = real * k - imag * e_seq;
        seq_t new_imag = real * e_seq + imag * k;
        real = new_real;
        imag = new_imag;
    }

    /* odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t k = prime_pi(p);   /* π(p) */
            seq_t e_seq = (seq_t)e;
            seq_t new_real = real * k - imag * e_seq;
            seq_t new_imag = real * e_seq + imag * k;
            real = new_real;
            imag = new_imag;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t k = prime_pi(m);   /* π(m) */
        seq_t e_seq = (seq_t)1;
        seq_t new_real = real * k - imag * e_seq;
        seq_t new_imag = real * e_seq + imag * k;
        real = new_real;
        imag = new_imag;
    }

    return real;
}
