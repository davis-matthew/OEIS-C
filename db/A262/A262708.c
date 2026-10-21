/*
 * A262708  a(n) = p-(p/5) where p = prime(n) and (p/5) is a Legendre symbol.
 * https://oeis.org/A262708
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p - (p/5) where p = prime(n) and (p/5) is the Legendre
 *            symbol  * The function computes the pth prime via get_prime and
 *            evaluates the * Legendre symbol modulo 5
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 4)          /* offset is 4; below this the value is undefined */
        return 0;

    /* get the nth prime (p is less than 2^32 for n≈10000) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Legendre symbol (p/5) */
    int legendre;
    if (p32 == 5)
        legendre = 0;
    else {
        uint32_t r = p32 % 5;
        legendre = (r == 1 || r == 4) ? 1 : -1;
    }

    /* compute p - (p/5) without using signed arithmetic. */
    seq_t result;
    if (legendre == 1)
        result = p - 1;
    else if (legendre == 0)
        result = p;
    else            /* legendre == -1 */
        result = p + 1;

    return result;
}
