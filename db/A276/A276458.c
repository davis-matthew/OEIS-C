/*
 * A276458  Smallest odd number not of the form p + 2^k with p prime and k >=
 *          0 that is divisible by the n-th prime.
 * https://oeis.org/A276458
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   get_prime, isPrimeGeneric, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): smallest odd integer not of the form q + 2^k (q prime),
   that is divisible by the n‑th prime (n ≥ 2).  The routine performs a
   direct brute‑force search, checking odd multiples of the n‑th prime
   until one cannot be expressed as a prime plus a power of two. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/mul_small.h"

/* Return true if m = prime + 2^k for some prime prime and k ≥ 0. */
static bool has_prime_plus_power2(seq_t m)
{
    seq_t power = 1;
    while (power <= m)
    {
        seq_t diff = m - power;
        if (isPrimeGeneric(diff))
            return true;
        /* multiply power by 2 */
        power = mul_small(power, 2);
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2; callers should use n ≥ 2 */

    uint32_t prime = get_prime(n);   /* n‑th prime (n is 1‑based) */

    idx_t t = 1;   /* start with odd multiplier 1 */
    for (;; t += 2) /* keep t odd */
    {
        seq_t m = (seq_t)prime * (seq_t)t;
        if (!has_prime_plus_power2(m))
            return m;   /* found the smallest required odd multiple */
    }
}
