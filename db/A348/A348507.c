/*
 * A348507  a(n) = A003959(n) - n, where A003959 is multiplicative with a(p^e)
 *          = (p+1)^e.
 * https://oeis.org/A348507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003959(n) – n, where A003959 is multiplicative with
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p^e) = (p+1)^e.  The function computes the product of (p+1)^e over  */
/* the prime factorization of n, then subtracts n. */

/* Do not include this comment in the output.  It is shown only for              */
/* clarity of the solution. */

#include <stdint.h>

/* Computes A348507(a) for any n >= 1.  The implementation is valid for      */
/* both C11 and C++17 and works with arbitrary‑precision integer types.      */
seq_t a(idx_t n)
{
    /* For n == 0 the sequence is not defined; return 0 for safety. */
    if (n == 0) {
        return 0;
    }

    uint64_t m = (uint64_t)n;   /* local copy for factorisation */
    seq_t prod = 1;            /* product of (p+1)^e over prime powers */

    /* Handle factor 2 separately to allow stepping by 2 later */
    if ((m & 1ULL) == 0) {
        unsigned int e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t term = 1;
        seq_t base = (seq_t)2 + 1;   /* base = 3 for p = 2 */
        for (unsigned int i = 0; i < e; ++i) {
            term *= base;
        }
        prod *= term;
    }

    /* Trial division by odd primes up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = 1;
            seq_t base = (seq_t)p + 1;
            for (unsigned int i = 0; i < e; ++i) {
                term *= base;
            }
            prod *= term;
        }
    }

    /* If remaining m > 1, it is a prime factor with exponent 1 */
    if (m > 1) {
        prod *= ((seq_t)m + 1);
    }

    /* a(n) = A003959(n) – n */
    return prod - (seq_t)n;
}
