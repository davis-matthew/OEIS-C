/*
 * A370157  Primes p such that both 3p+2 and (p-2)/3 are composite or 0.
 * https://oeis.org/A370157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370157:
   a(n) = the n-th prime p such that 3p+2 is composite and
   (p-2)/3 is composite or equals 0.
   Computation: iterate primes and test the two conditions. */

#include <stdbool.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many qualifying primes found */
    uint64_t p = 2;           /* candidate prime to test */

    while (found < n) {
        if (is_prime(p)) {            /* p is prime */
            /* condition 1: 3p+2 must be composite */
            uint64_t m = 3 * p + 2;
            if (is_prime(m)) {
                p++;
                continue;
            }

            /* condition 2: (p-2)/3 must be composite or 0 */
            bool ok = true;
            if ((p >= 2) && ((p - 2) % 3 == 0)) {
                uint64_t k = (p - 2) / 3;   /* integer division */
                if (k >= 2 && is_prime(k)) {
                    ok = false;            /* k is prime → reject */
                }
            }

            if (ok) {
                found++;
                if (found == n) {
                    /* seq_t may be narrower than uint64_t; cast safely. */
                    return (seq_t)p;
                }
            }
        }
        p++;
    }

    /* This point should never be reached for valid input. */
    return (seq_t)0;
}
