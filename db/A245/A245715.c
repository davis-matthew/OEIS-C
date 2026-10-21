/*
 * A245715  Least number k > 0 such that n - k! is prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A245715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1818, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least k>0 such that n - k! is prime, or 0 if none exists. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* For n >= 1, factorial grows rapidly; any valid k satisfies k! < n.
       We try successive k, compute k! incrementally, and check primality of
       n - k!. */
    seq_t fact = 1;              /* current k! */
    for (idx_t k = 1; ; ++k) {
        fact *= (seq_t)k;        /* fact now equals k! */
        if (fact >= (seq_t)n)    /* k! >= n → no larger k can satisfy n - k! > 0 */
            break;
        seq_t m = (seq_t)n - fact;
        if (is_prime_small(m))
            return (seq_t)k;
    }
    return 0;                    /* no suitable k found */
}
