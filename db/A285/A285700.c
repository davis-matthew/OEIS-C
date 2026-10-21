/*
 * A285700  a(n) = Number of iterations x -> 2x-1 needed to get a nonprime
 *          number, when starting with x = n.
 * https://oeis.org/A285700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is composite; otherwise a(n) = 1 + a(2*n-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (!is_prime(n))
        return 0;
    idx_t m = 2*n - 1;
    return (seq_t)1 + a(m);
}
