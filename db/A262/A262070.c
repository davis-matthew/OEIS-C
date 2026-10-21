/*
 * A262070  a(n) = ceiling( log_3( binomial(n,2) ) ).
 * https://oeis.org/A262070
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(log_3( n*(n-1)/2 ))
 * Range:     u8 n <= 22, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For the sequence offset 2, we treat n<2 as returning 0. */
    if (n < 2) {
        return 0;
    }

    /* Compute binomial(n,2) in seq_t: n*(n-1)/2. */
    seq_t binom = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Find the smallest k with 3^k >= binom. */
    seq_t power = 1;
    idx_t k = 0;
    while (power < binom) {
        power *= 3;
        ++k;
    }

    return (seq_t)k;   /* ceil(log_3(binomial)) */
}
