/*
 * A331410  a(n) is the number of iterations needed to reach a power of 2
 *          starting at n and using the map k -> k + k/p, where p is the
 *          largest prime factor of k.
 * https://oeis.org/A331410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations to reach a power of two using the
 *            map k -> k + k/p where p is the largest prime factor of k.
 *            Recurrence: a(n) = 0 if n is a power of two, otherwise a(n) = 1
 *            + a( n + n / lpf(n) )
 * Range:     u8 n <= 74, u16 n <= 9290, u32 n <= 9290, u64 n <= 9290,
 *            u128 n <= 9290, big any n
 * Helpers:   is_power_of_two, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/largest_prime_factor.h" /* provides largest_prime_factor */
#include "../utils/is_power_of_two.h"       /* provides is_power_of_two */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0 and a(0) is 0 by definition */

    seq_t curr = n;
    seq_t cnt  = 0;

    /* Iterate until curr becomes a power of two */
    while (!is_power_of_two(curr)) {
        uint64_t p = largest_prime_factor((uint64_t)curr);
        /* p is guaranteed to be > 1 here; curr is odd or has odd part > 1 */
        curr += curr / p;          /* k -> k + k/p */
        ++cnt;
    }

    return cnt;
}
