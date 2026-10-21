/*
 * A273711  The Hadamard product of omega(n) and A007875(n).
 * https://oeis.org/A273711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = omega(n) * 2^(omega(n)-1)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Count distinct prime factors of n (omega(n)) */
    int w_int = count_distinct_primes((uint64_t)n);
    if (w_int == 0)
        return 0;    /* Handling n = 1 */

    /* Compute 2^(omega(n)-1) */
    seq_t pow = pow2((idx_t)(w_int - 1));

    /* Return omega(n) * 2^(omega(n)-1) */
    return (seq_t)w_int * pow;
}
