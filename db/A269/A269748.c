/*
 * A269748  a(n) = 2*p+61+2*gcd(p-1,3)+gcd(p-1,4), where p = prime(n).
 * https://oeis.org/A269748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n) + 61 + 2*gcd(prime(n)-1,3) + gcd(prime(n)-1,4)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* p = the nth prime (offset 1) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* base value 2*p + 61 */
    seq_t val = 2 * p + 61;

    /* add 2*gcd(p-1,3) + gcd(p-1,4) */
    seq_t g3 = gcd(p - 1, 3);
    seq_t g4 = gcd(p - 1, 4);

    val += 2 * g3 + g4;
    return val;
}
