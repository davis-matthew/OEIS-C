/*
 * A353672  a(n) = 1 if n is an even number with an even number of distinct
 *          prime factors, otherwise 0.
 * https://oeis.org/A353672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n is even] * [A001221(n) even]
 * Range:     u8 n <= 9537, u16 n <= 9537, u32 n <= 9537, u64 n <= 9537,
 *            u128 n <= 9537, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {               /* n is even */
        uint64_t d = count_distinct_primes(n);  /* distinct prime factors */
        if ((d & 1ULL) == 0)               /* even number of them */
            return (seq_t)1;
    }
    return (seq_t)0;
}
