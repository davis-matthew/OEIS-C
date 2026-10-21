/*
 * A248211  First differences of omega(n), the number of distinct prime
 *          factors function (A001221).
 * https://oeis.org/A248211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = omega(n+1) - omega(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* OEIS offset 1: the sequence is defined for n >= 1. */
    if (n == 0)
        return 0;   /* behaviour for n < 1 is irrelevant */

    uint64_t nn = (uint64_t)n;

    /* Count distinct prime factors (omega) for n and n+1 */
    int omega_n   = count_distinct_primes(nn);
    int omega_np1 = count_distinct_primes(nn + 1);

    return (seq_t)omega_np1 - (seq_t)omega_n;
}
