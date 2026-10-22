/*
 * A373978  a(n) = 1 if n is a nonprime of the form 4*k + 1, otherwise 0.
 * https://oeis.org/A373978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 1 or (n % 4 == 1 and n is not prime), otherwise
 *            0
 * Range:     u8 n <= 9944, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if ((n % 4ULL) == 1ULL && !is_prime(n))
        return (seq_t)1;
    return (seq_t)0;
}
