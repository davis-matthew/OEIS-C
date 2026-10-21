/*
 * A244796  Number of moduli m such that (prime(n) mod m) = 1.
 * https://oeis.org/A244796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(prime(n)-1) - 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; just in case */

    /* Obtain the n‑th prime. */
    uint32_t p = get_prime(n);

    /* Count divisors of (prime(n)-1). */
    uint64_t val = (uint64_t)p - 1ULL;
    seq_t cnt = (seq_t)divisor_count_u64(val);

    /* Subtract 1 as per the definition. */
    return cnt - 1;
}
