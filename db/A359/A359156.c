/*
 * A359156  a(n) = 1 if the odd part of n is squarefree and the number of
 *          prime factors of n (with multiplicity) is even, otherwise 0.
 * https://oeis.org/A359156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is squarefree and the total number
 *            of prime factors of n (counted with multiplicity) is even,
 *            otherwise 0
 * Range:     u8 n <= 3421, u16 n <= 9406, u32 n <= 9406, u64 n <= 9406,
 *            u128 n <= 9406, big any n
 * Helpers:   count_prime_factors, is_squarefree, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"
#include "../utils/odd_part.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; the function may safely be called with n>=1. */
    seq_t odd = odd_part((seq_t)n);

    if (!is_squarefree(odd))
        return (seq_t)0;

    seq_t pf = count_prime_factors((seq_t)n);
    return (pf & (seq_t)1) == 0 ? (seq_t)1 : (seq_t)0;
}
