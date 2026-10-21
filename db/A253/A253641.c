/*
 * A253641  Largest integer b such that n=a^b for some integer a; a(0)=a(1)=1
 *          by convention.
 * https://oeis.org/A253641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest b such that n = a^b for some integer a; a(0) =
 *            a(1) = 1 (defined by convention)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_gcd.h"

/* Euclidean algorithm for the greatest common divisor of two non‑negative
   integers.  The exponents are at most 64, so 32‑bit int is sufficient. */

seq_t a(idx_t n)
{
    /* Handle the special cases n = 0 and n = 1. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    idx_t temp = n;          /* number to be factored */
    int g = 0;                /* current greatest common divisor of exponents */

    /* Handle factor 2 separately to make the loop step even. */
    if ((temp & 1) == 0) {    /* temp % 2 == 0 */
        int e = 0;
        while ((temp & 1) == 0) {   /* while temp % 2 == 0 */
            temp >>= 1;             /* temp /= 2 */
            ++e;
        }
        g = e;
    }

    /* Trial division by odd primes. */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            g = (g == 0) ? e : int_gcd(g, e);
        }
    }

    /* If what remains is a prime > 2, its exponent is 1. */
    if (temp > 1) {
        g = (g == 0) ? 1 : int_gcd(g, 1);
    }

    return (seq_t)g;
}
