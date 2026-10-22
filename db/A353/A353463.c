/*
 * A353463  Characteristic function of lesser twin primes: a(n) = 1 if both n
 *          and n+2 are primes, otherwise 0.
 * https://oeis.org/A353463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9634, u16 n <= 9634, u32 n <= 9634, u64 n <= 9634,
 *            u128 n <= 9634, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if both n and n+2 are primes, otherwise 0
   (characteristic function of lesser twin primes) */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* guard against invalid indices and overflow */
    if (n == 0 || n > UINT64_MAX - 2)
        return (seq_t)0;

    return (is_prime(n) && is_prime(n + 2)) ? (seq_t)1 : (seq_t)0;
}
