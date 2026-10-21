/*
 * A271664  Erroneous version of A271811 (but for odd primes only).
 * https://oeis.org/A271664
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*p^2 + 145*p + 1338 + 80*gcd(p-1,3) + 45*gcd(p-1,4) +
 *            8*gcd(p-1,5) + 8*gcd(p-1,6))/4 for n>2 (where p = prime(n)),
 *            a(2) = 491
 * Range:     u8 none, u16 n <= 18, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined below the offset */

    if (n == 2)                 /* special first term */
        return (seq_t)491;

    /* p = the n-th prime (prime(1) = 2, which is not used for n>=2) */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Compute the polynomial part */
    seq_t p2  = p * p;
    seq_t sum = (seq_t)13 * p2 + (seq_t)145 * p + (seq_t)1338;

    /* Add the gcd terms */
    seq_t g3 = gcd(p - (seq_t)1, (seq_t)3);
    seq_t g4 = gcd(p - (seq_t)1, (seq_t)4);
    seq_t g5 = gcd(p - (seq_t)1, (seq_t)5);
    seq_t g6 = gcd(p - (seq_t)1, (seq_t)6);

    sum += (seq_t)80 * g3;
    sum += (seq_t)45 * g4;
    sum += (seq_t)8  * g5;
    sum += (seq_t)8  * g6;

    return sum / (seq_t)4;
}
