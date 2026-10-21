/*
 * A276088  The least significant nonzero digit in primorial base
 *          representation of n: a(n) = A276094(n) / A002110(A276084(n)) (with
 *          a(0) = 0).
 * https://oeis.org/A276088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least significant nonzero digit of the primorial base *
 *            representation of n. The algorithm repeatedly divides n by the *
 *            successive primes 2,3,5,7,…; when a prime does not divide the *
 *            current quotient the remainder of that division is the requested
 *            * digit
 * Range:     u8 n <= 2033, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t k = (seq_t)n;
    for (idx_t idx = 1; ; ++idx) {
        uint32_t prime = get_prime(idx);          /* 1 → 2, 2 → 3, etc. */
        seq_t r = k % (seq_t)prime;
        if (r == 0)
            k = k / (seq_t)prime;                /* divide if divisible   */
        else
            return r;                            /* first non‑zero digit  */
    }
}
