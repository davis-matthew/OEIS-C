/*
 * A262608  Primes p such that floor(10*p/Pi) mod 10 = 0.
 * https://oeis.org/A262608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that floor(10*p/π) mod 10 = 0
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th prime in that sequence. */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"      /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    static const long double pi = acosl(-1.0L);   /* π with long double precision */
    idx_t found = 0;          /* how many terms we have found so far */
    uint64_t p = 2;           /* current candidate integer (start with the first prime) */

    for (;;) {
        if (is_prime(p)) {
            /* compute floor(10*p/π) using long double arithmetic */
            long double v = floorl(10.0L * (long double)p / pi);
            if ((uint64_t)v % 10ULL == 0ULL) {
                ++found;
                if (found == n)
                    return (seq_t)p;            /* cast to seq_t before returning */
            }
        }
        ++p;   /* move to next integer */
    }
}
