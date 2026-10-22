/*
 * A331044  a(n) is the greatest prime number of the form floor(n/10^k) for
 *          some k >= 0, or 0 if no such prime number exists.
 * https://oeis.org/A331044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{ p | p is prime and p = floor(n / 10^k) for some k >=
 *            0 }
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t p = n;
    while (p > 0) {
        if (is_prime(p))
            return (seq_t)p;
        p /= 10;
    }
    return (seq_t)0;
}
