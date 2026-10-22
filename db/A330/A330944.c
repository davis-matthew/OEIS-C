/*
 * A330944  Number of nonprime prime indices of n.
 * https://oeis.org/A330944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime indices of n that are nonprime, where a
 *            prime index of n is a number m such that prime(m) divides n. For
 *            each prime factor p^e of n, let k = prime index of p (k =
 *            prime_pi(p)); if k is not a prime, add e to the result
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Array sizes are safely large for any 64‑bit integer factorisation. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* Compute the prime index of the prime factor primes[i]. */
        seq_t pi_val = prime_pi(primes[i]);          /* prime_pi returns seq_t          */
        idx_t  idx   = (idx_t)pi_val;                /* convert to idx_t for primality test */

        if (!is_prime(idx))                          /* idx is nonprime -> count it   */
            result += (seq_t)exps[i];
    }
    return result;
}
