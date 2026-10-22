/*
 * A344349  Number of primes along the main antidiagonal of the n X n square
 *          array whose elements are the numbers from 1..n^2, listed in
 *          increasing order by rows.
 * https://oeis.org/A344349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} c(k*(n-1)+1)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t step = n - 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t val = k * step + 1;
        if (is_prime(val))
            ++count;
    }
    return count;
}
