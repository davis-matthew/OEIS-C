/*
 * A378366  Difference between n and the greatest non prime power <= n
 *          (allowing 1).
 * https://oeis.org/A378366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A378367(n) * where A378367(n) is the greatest non
 *            prime power <= n (allowing 1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

/* Check whether m is a prime power (p^k for k>=1).  1 is NOT a prime power. */
static bool _is_prime_power(uint64_t m)
{
    if (m <= 1) return false;          /* 1 and 0 are not prime powers */
    return count_distinct_primes(m) == 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                /* a(1)=0, a(0) irrelevant */

    uint64_t m = (uint64_t)n;
    while (m > 1) {
        if (!_is_prime_power(m))
            break;                     /* found greatest non prime power */
        --m;
    }
    /* For n>=2, m will be at least 1 */
    return (seq_t)n - (seq_t)m;
}
