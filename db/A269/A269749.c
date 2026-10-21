/*
 * A269749  a(n) = 3*p^2+39*p+344+24*gcd(p-1,3)+11*gcd(p-1,4)+2*gcd(p-1,5),
 *          where p = prime(n).
 * https://oeis.org/A269749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p^2+39*p+344+24*gcd(p-1,3)+11*gcd(p-1,4)+2*gcd(p-1,5),
 *            where p = prime(n)
 * Range:     u8 none, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;              /* undefined for n < 1, return 0 */

    /* Obtain the nth prime.  get_prime returns a 32‑bit unsigned integer. */
    uint32_t p_u32 = get_prime(n);
    seq_t p          = (seq_t)p_u32;   /* cast to the required seq_t type */

    /* Compute the main part of the formula.  All arithmetic is performed
       in seq_t, which may truncate if the value does not fit. */
    seq_t term = (seq_t)3 * p * p;
    term += (seq_t)39 * p;
    term += (seq_t)344;

    /* Add the gcd terms. */
    seq_t gp1 = (seq_t)1;
    seq_t g3 = gcd(p - gp1, (seq_t)3);
    seq_t g4 = gcd(p - gp1, (seq_t)4);
    seq_t g5 = gcd(p - gp1, (seq_t)5);

    term += (seq_t)24 * g3;
    term += (seq_t)11 * g4;
    term += (seq_t) 2 * g5;

    return term;
}
