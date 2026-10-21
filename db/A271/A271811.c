/*
 * A271811  Number of non-abelian groups of order prime(n)^6.
 * https://oeis.org/A271811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non-abelian groups of order prime(n)^6
 * Range:     u8 none, u16 n <= 34, u32 n <= 1229, u64 n <= 1229,
 *            u128 n <= 1229, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the explicit closed form for p > 3, with
 * special cases for p = 2 and p = 3. */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* special cases for p = 2 and p = 3 */
    if (p32 == 2) return (seq_t)256;        /* 267 total groups – 11 abelian */
    if (p32 == 3) return (seq_t)493;        /* 504 total groups – 11 abelian */

    /* p > 3 : use the explicit formula  
       a(p) = 3p^2 + 39p + 333 + 24 gcd(p-1,3) +
              11 gcd(p-1,4) + 2 gcd(p-1,5)   */
    seq_t p_minus1 = p - 1;

    seq_t result = 3 * p * p;                    /* 3p^2 */
    result += 39 * p;                            /* +39p */
    result += (seq_t)333;                        /* +333 */

    result += 24 * gcd(p_minus1, (seq_t)3);      /* +24 gcd(p-1,3) */
    result += 11 * gcd(p_minus1, (seq_t)4);      /* +11 gcd(p-1,4) */
    result += 2  * gcd(p_minus1, (seq_t)5);      /* +2  gcd(p-1,5) */

    return result;
}
