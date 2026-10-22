/*
 * A360825  a(n) is the remainder after dividing n! by its least nondivisor.
 * https://oeis.org/A360825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! mod L(n) where L(n) is the least nondivisor of n!
 *            (next prime after n, except L(0)=L(1)=2, L(2)=3, L(3)=4)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fact_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/fact_mod.h"

/* Simple primality test sufficient for the size of n needed here. */
static bool is_prime(uint64_t x)
{
    if (x < 2)                return false;
    if (x == 2 || x == 3)     return true;
    if ((x & 1) == 0)         return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)       return false;
    return true;
}

/* Return the smallest prime greater than n. */
static uint64_t next_prime_uint(uint64_t n)
{
    uint64_t p = n + 1;
    while (!is_prime(p)) ++p;
    return p;
}

seq_t a(idx_t n)
{
    uint64_t L;
    if (n <= 1)          L = 2;          /* n = 0 or 1 */
    else if (n == 2)     L = 3;
    else if (n == 3)     L = 4;
    else                 L = next_prime_uint((uint64_t)n);

    return fact_mod(n, (seq_t)L);
}
