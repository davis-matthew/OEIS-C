/*
 * A292438  Characteristic function of non-isolated nonprimes.
 * https://oeis.org/A292438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 0; for n > 2: a(n) = 1 - is_prime(n -
 *            ((n+1) % 2)) * is_prime(n + ((n+1) % 2)) where is_prime is the
 *            helper from utils/is_prime.h (A010051 in OEIS)
 * Range:     u8 n <= 9655, u16 n <= 9655, u32 n <= 9655, u64 n <= 9655,
 *            u128 n <= 9655, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
/* utility header providing is_prime(idx_t) */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the special cases explicitly */
    if (n == 0 || n == 1)
        return 1;           /* 0 and 1 are non‑primes */
    if (n == 2)
        return 0;           /* 2 is the unique even prime */

    /* For odd n: a(n) = 1 - is_prime(n) */
    if (n & 1U)
        return 1 - (seq_t)is_prime(n);

    /* For even n > 2: a(n) = 1 - (is_prime(n-1) && is_prime(n+1)) */
    return 1 - (seq_t)(is_prime(n-1) && is_prime(n+1));
}
