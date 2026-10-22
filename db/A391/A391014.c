/*
 * A391014  Indices of zeroless primes.
 * https://oeis.org/A391014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index of the n‑th prime that contains no zero digit
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return true if a decimal representation of n contains a zero digit. */
static bool has_zero_digit(uint64_t n)
{
    if (n == 0) return true;          /* just in case */
    while (n != 0ULL)
    {
        if ((n % 10ULL) == 0ULL)
            return true;
        n /= 10ULL;
    }
    return false;
}

/* Return the index of the n‑th zeroless prime. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n==0, return 0 for safety */
        return 0;

    uint64_t candidate = 2;   /* the first prime */
    idx_t   prime_idx   = 0;   /* 1‑based index of the current prime */
    idx_t   zerofree    = 0;   /* count of primes without zero digit */

    while (true)
    {
        if (is_prime(candidate))
        {
            prime_idx++;              /* prime index increments for every prime */

            if (!has_zero_digit(candidate))
            {
                zerofree++;            /* count a zeroless prime */
                if (zerofree == n)     /* found the n‑th zeroless prime */
                    return (seq_t)prime_idx;
            }
        }

        /* Next candidate: 2→3, then all odd numbers */
        if (candidate == 2ULL)
            candidate = 3ULL;
        else
            candidate += 2ULL;
    }
}
