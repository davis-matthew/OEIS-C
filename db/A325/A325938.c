/*
 * A325938  a(n) = omega(n)^tau(n), where omega=A001221 and tau=A000005.
 * https://oeis.org/A325938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(n)^tau(n)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 209, u64 n <= 839,
 *            u128 n <= 7559, big any n
 * Helpers:   big_pow, count_distinct_primes, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* omega(n) is the number of distinct prime factors of n (A001221) */
/* tau(n) is the number of divisors of n (A000005) */

#include "../utils/count_distinct_primes.h"   /* count_distinct_primes(uint64_t n) -> int */
#include "../utils/divisor_count_u64.h"       /* divisor_count_u64(uint64_t n) -> uint64_t */
#include "../utils/big_pow.h"                 /* big_pow(seq_t base, seq_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* Compute omega(n) and tau(n) using available helpers */
    int omega   = count_distinct_primes((uint64_t)n);
    uint64_t tau = divisor_count_u64((uint64_t)n);

    /* Return omega(n) raised to the power tau(n) */
    return big_pow((seq_t)omega, (seq_t)tau);
}
