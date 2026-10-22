/*
 * A323305  Number of divisors of the number of prime factors of n counted
 *          with multiplicity.
 * https://oeis.org/A323305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A001222(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   count_prime_factors, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prime_factors.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* by definition, a(1) is 1. */
    if (n == 1)
        return (seq_t)1;

    /* Ω(n) : total number of prime factors of n (with multiplicity). */
    seq_t omega = count_prime_factors((seq_t)n);

    /* ω(n) is always small (≤ log₂ n), so we may safely cast it to uint64_t. */
    uint64_t om = (uint64_t)omega;

    /* Number of positive divisors of ω(n). */
    return (seq_t)divisor_count_u64(om);
}
