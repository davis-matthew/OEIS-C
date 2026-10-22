/*
 * A358272  Multiplicative sequence with a(p^e) = (-1)^e * p^(2*floor(e/2))
 *          for prime p and e >= 0.
 * https://oeis.org/A358272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (-1)^e * p^(2 * floor(e/2))
 * Range:     i8 n <= 143, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Multiplicative sequence – use prime factorisation of n) */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* below the OEIS offset – arbitrary */
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Process factor 2 separately */
    unsigned exp2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp2++;
    }
    if (exp2 > 0) {
        seq_t term = 1;
        unsigned e = exp2;
        unsigned e_pow = e - (e & 1);          /* e if even, e-1 if odd */
        for (unsigned i = 0; i < e_pow; ++i)
            term *= (seq_t)2;
        if (e & 1)
            term = -term;
        result *= term;
    }

    /* Process odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t term = 1;
            unsigned e_pow = e - (e & 1);
            for (unsigned i = 0; i < e_pow; ++i)
                term *= (seq_t)p;
            if (e & 1)
                term = -term;
            result *= term;
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1) {
        unsigned e = 1;                     /* exponent of the remaining prime is 1 */
        seq_t term = 1;                     /* e_pow = 0, so term stays 1 before sign */
        if (e & 1)
            term = -term;                   /* (-1)^1 = -1 */
        result *= term;
    }

    return result;
}
