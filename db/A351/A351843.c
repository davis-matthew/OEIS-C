/*
 * A351843  a(n) = sigma(n) * sopf(n).
 * https://oeis.org/A351843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * sopf(n)
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where sigma(n) = sum of all divisors of n */
/*        sopf(n) = sum of distinct prime divisors of n */

#include "../utils/sigma_of.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    if (n == 1) return 0;         /* a(1) = 0 */

    uint64_t temp = (uint64_t)n;  /* safe cast for 64‑bit work */
    seq_t sopf = 0;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        sopf += (seq_t)2;
        while ((temp & 1ULL) == 0) temp >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            sopf += (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) sopf += (seq_t)temp;

    seq_t sigma = sigma_of(n);    /* built‑in utility for sigma(n) */
    return sigma * sopf;
}
