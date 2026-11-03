/*
 * A306224  a(n) is the smallest prime p such that -n is not a square mod p.
 * https://oeis.org/A306224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   is_prime
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
#include <stdint.h>
#include "../utils/is_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
