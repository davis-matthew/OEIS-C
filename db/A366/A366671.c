/*
 * A366671  Smallest prime dividing 8^n + 1.
 * https://oeis.org/A366671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime divisor of 8^n + 1
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 127, u64 n <= 9694,
 *            u128 n <= 9694, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                                                       */
/* For the special cases:                                               */
/*   a(0) = 2                                                         */
/*   a(n) = 3  if n is odd                                           */
/*   a(n) = 5  if n ≡ 2 (mod 4)                                       */
/* For n ≡ 0 (mod 4) write a(n) as the first prime p with              */
/*   8^n ≡ –1 (mod p).  That prime is the smallest divisor of         */
/*   8^n + 1.                                                           */
/*                                                                       */
/* The implementation uses a simple trial‑division primality test        */
/* and the provided mod‑pow routine.  No external static state is       */
/* required – the search for a prime factor is performed lazily.        */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Simple trial‑division test for uint64_t primes. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
    {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0)                    return (seq_t)2;
    if ((n & 1) == 1)              return (seq_t)3;
    if ((n & 3) == 2)              return (seq_t)5;

    /* general case: n is a multiple of 4.  Search for the smallest
       odd prime p such that 8^n ≡ –1 (mod p). */
    for (uint64_t p = 3; ; p += 2)
    {
        if (!is_prime_u64(p)) continue;
        seq_t mod = (seq_t)p;
        seq_t r   = mod_pow((seq_t)8, n, mod);
        if (r == mod - (seq_t)1) return (seq_t)p;
    }
}
