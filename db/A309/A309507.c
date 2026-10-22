/*
 * A309507  Number of ways the n-th triangular number T(n) = A000217(n) can be
 *          written as the difference of two positive triangular numbers.
 * https://oeis.org/A309507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d_odd(n) * d_odd(n+1) - 1 * where d_odd(k) is the number
 *            of odd divisors of k (including 1). * Since n and n+1 are
 *            coprime, d_odd(n*(n+1)) = d_odd(n)*d_odd(n+1). * The desired
 *            count excludes the divisor 1, hence the final "- 1"
 * Range:     u8 n <= 8827, u16 n <= 8827, u32 n <= 8827, u64 n <= 8827,
 *            u128 n <= 8827, big any n
 * Helpers:   count_odd_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdint.h>  /* for uint64_t, if needed */
#include "../utils/count_odd_divisors.h"

/* OEIS A309507: number of ways T(n) can be expressed as difference of two positive triangular numbers. */
seq_t a(idx_t n)
{
    if (n == 0)        /* not required for offset 1, but safe guard */
        return 0;

    seq_t d1 = count_odd_divisors(n);
    seq_t d2 = count_odd_divisors(n + 1);

    return d1 * d2 - 1;
}
