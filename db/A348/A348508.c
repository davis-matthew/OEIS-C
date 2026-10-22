/*
 * A348508  a(n) = A003959(n) - 2*n, where A003959 is multiplicative with
 *          a(p^e) = (p+1)^e.
 * https://oeis.org/A348508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003959(n) - 2*n
 * Range:     i8 n <= 31, i16 n <= 1023, i32 n <= 8751, i64 n <= 8751,
 *            i128 n <= 8751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = A003959(n) - 2*n,
   where A003959 is multiplicative with  A003959(p^e) = (p+1)^e. */
seq_t a(idx_t n)
{
    seq_t prod = 1;
    uint64_t temp = (uint64_t)n;

    /* factor out the power of 2 */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e) {
        seq_t base = (seq_t)(2 + 1);          /* 3 */
        seq_t pow  = 1;
        for (uint64_t i = 0; i < e; ++i)
            pow *= base;
        prod *= pow;
    }

    /* factor odd primes by trial division */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t base = (seq_t)(p + 1);
            seq_t pow  = 1;
            for (uint64_t i = 0; i < e; ++i)
                pow *= base;
            prod *= pow;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t base = (seq_t)(temp + 1);
        prod *= base;
    }

    seq_t result = prod - (seq_t)2 * (seq_t)n;
    return result;
}
