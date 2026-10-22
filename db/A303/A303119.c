/*
 * A303119  Number of partitions of n into two parts (p,q) with p <= q such
 *          that p is semiprime and q is squarefree.
 * https://oeis.org/A303119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1..floor(n/2)} [count_prime_factors(i) == 2 &&
 *            is_squarefree(n-i)]
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   count_prime_factors, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/count_prime_factors.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        if ((count_prime_factors((seq_t)i) == 2) &&
            is_squarefree((seq_t)(n - i))) {
            ++result;
        }
    }
    return result;
}
