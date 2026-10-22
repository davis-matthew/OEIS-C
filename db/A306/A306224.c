/*
 * A306224  a(n) is the smallest prime p such that -n is not a square mod p.
 * https://oeis.org/A306224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function implements the definition of OEIS A306224:
 *
 *  a(n) is the smallest odd prime p such that -n is not a quadratic
 *  residue modulo p.  In other words Legendre(-n, p) = -1.
 *  This is equivalent to the Jacobi symbol (p - (n mod p) | p) = -1 for an odd prime p.
 *
 *  The algorithm simply tests odd primes in increasing order until it finds
 *  one for which the Jacobi symbol equals -1.  The test of primality uses
 *  the generic `is_prime` helper provided by the build utilities.
 */

#include <stdbool.h>
#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t a, uint64_t n); */
#include "../utils/is_prime.h"         /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 immediately. */
    if (n == 1) return (seq_t)3;

    /* Search odd primes starting at 3. */
    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime(p)) continue;           /* skip non‑primes */
        if (n % p == 0) continue;             /* Legendre symbol would be 0 */

        uint64_t mod = n % p;                 /* n mod p */
        uint64_t x   = p - mod;               /* -n mod p (non‑zero) */

        /* Jacobi symbol equals Legendre symbol when p is prime. */
        int js = jacobi_symbol(x, p);          /* js ∈ {−1, 0, 1} */

        if (js == -1) return (seq_t)p;         /* found the required prime */
    }
}
