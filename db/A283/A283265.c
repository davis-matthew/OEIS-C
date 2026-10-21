/*
 * A283265  a(n) = 1 if n is neither 2 nor a lesser or greater twin prime (in
 *          A001097), 0 otherwise.
 * https://oeis.org/A283265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is neither 2 nor a twin prime; 0 otherwise
 * Range:     u8 n <= 9125, u16 n <= 9125, u32 n <= 9125, u64 n <= 9125,
 *            u128 n <= 9125, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;      /* a(1) = 1 */
    if (n == 2) return (seq_t)0;      /* 2 is excluded */

    /* If n is prime, check for twin primes */
    if (is_prime(n)) {
        /* Check lower twin */
        if (n >= 3 && is_prime(n - 2))
            return (seq_t)0;

        /* Check upper twin, guard against unsigned overflow */
        if (n <= UINT64_MAX - 2 && is_prime(n + 2))
            return (seq_t)0;
    }

    /* All other numbers, including composites and 1, return 1 */
    return (seq_t)1;
}
