/*
 * A359465  a(n) = 1 if n is an odd squarefree number with an even number of
 *          prime factors, otherwise 0.
 * https://oeis.org/A359465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is an odd squarefree integer with an even number
 *            of distinct prime factors, otherwise 0
 * Range:     u8 n <= 9954, u16 n <= 9954, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) /* even n */
        return (seq_t)0;

    seq_t x = (seq_t)n;
    if (!is_squarefree(x))
        return (seq_t)0;

    int omega = count_distinct_primes(n);
    return (seq_t)((omega & 1) == 0 ? 1 : 0);
}
