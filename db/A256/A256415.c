/*
 * A256415  Positive integers with primes p replaced by 2p and also 3p
 *          replaced by 2p.
 * https://oeis.org/A256415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementing A256415: replace every prime p by 2p,
   and replace every integer of the form 3p (p prime) by 2p. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* safety for out‐of‑range calls */
        return 0;

    if (is_prime(n))                 /* prime → 2p */
        return (seq_t)n * 2;

    if (n % 3 == 0)                   /* check 3p */
    {
        idx_t p = n / 3;
        if (is_prime(p))
            return (seq_t)p * 2;
    }

    return (seq_t)n;                   /* otherwise keep n */
}
