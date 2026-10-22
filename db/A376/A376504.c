/*
 * A376504  Number of divisors of n that are both composite and squarefree.
 * https://oeis.org/A376504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^omega(n) - omega(n) - 1 * where omega(n) is the number
 *            of distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"


seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥1 */
    uint64_t nn = (uint64_t)n;

    /* Compute ω(n) – the number of distinct prime factors. */
    size_t omega = count_distinct_primes(nn);

    /* For ω(n) = 0 or 1 the result is 0, so we can return early. */
    if (omega <= 1)
        return (seq_t)0;

    /* 2^ω(n) */
    seq_t pow2omega = pow2((idx_t)omega);

    /* a(n) = 2^ω(n) - ω(n) - 1 */
    seq_t result = pow2omega - (seq_t)omega - (seq_t)1;
    return result;
}
