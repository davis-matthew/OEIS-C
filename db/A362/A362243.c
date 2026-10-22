/*
 * A362243  a(n) = number of isomorphism classes of elliptic curves over the
 *          finite field of order prime(n).
 * https://oeis.org/A362243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=8, a(n)=2*prime(n)+C for n>2, * where C=6 if
 *            prime(n)%12==1, * C=2 if prime(n)%12==5, * C=4 if
 *            prime(n)%12==7, * C=0 if prime(n)%12==11
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;

    /* get the nth prime (1‑based) */
    uint32_t p = get_prime(n);

    seq_t val = (seq_t)p * (seq_t)2;
    uint32_t mod = p % 12u;
    seq_t C;
    switch (mod) {
        case 1: C = (seq_t)6; break;
        case 5: C = (seq_t)2; break;
        case 7: C = (seq_t)4; break;
        case 11: C = (seq_t)0; break;
        default: C = (seq_t)0; /* should not occur for primes >3 */
    }

    return val + C;
}
