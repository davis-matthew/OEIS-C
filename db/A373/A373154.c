/*
 * A373154  a(n) = 1 if 6*n is squarefree, otherwise 0.
 * https://oeis.org/A373154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if 6*n is squarefree, otherwise 0
 * Range:     u8 n <= 3888, u16 n <= 8899, u32 n <= 8899, u64 n <= 8899,
 *            u128 n <= 8899, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* A(n) is defined for n >= 1.  If n is divisible by 2 or 3,
       then 6*n has that prime squared, so it cannot be squarefree. */
    if ((n & 1) == 0 || (n % 3) == 0)
        return (seq_t)0;

    /* For all other primes p, 6*n inherits the multiplicity of p from n.
       Thus 6*n is squarefree iff n itself is squarefree. */
    return (seq_t)(is_squarefree((seq_t)n) ? 1 : 0);
}
