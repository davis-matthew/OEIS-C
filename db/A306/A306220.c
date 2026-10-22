/*
 * A306220  a(n) is the smallest prime p such that Kronecker(-n,p) = -1.
 * https://oeis.org/A306220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) = smallest prime p such that Kronecker(-n,p) = -1.
 * This is a direct search using Legendre symbols for odd primes
 * and the special rule for p = 2.  The search is independent of
 * call order and terminates in few milliseconds for n ≤ 10000.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/powmod64.h"

/* Legendre symbol (a|p) for odd prime p.
 * Returns 1 if a is a quadratic residue mod p,
 *        -1 if non‑residue,
 *         0 if a ≡ 0 (mod p). */
static int legendre_symbol(uint64_t a, uint64_t p)
{
    uint64_t mod = a % p;
    if (mod == 0)
        return 0;
    uint64_t exp = (p - 1) >> 1;                 /* (p-1)/2 */
    uint64_t r   = powmod64(mod, exp, p);        /* Euler's criterion */
    if (r == 1)
        return 1;
    if (r == p - 1)
        return -1;
    return 0;   /* unreachable for odd prime, kept for completeness */
}

seq_t a(idx_t n)
{
    uint64_t u64 = (uint64_t)n;                 /* work in 64‑bit */
    /* Special case p = 2 */
    if (u64 & 1) {                              /* n is odd */
        uint64_t mod8 = u64 % 8;
        if (mod8 == 3 || mod8 == 5)
            return (seq_t)2;                    /* Kronecker(-n,2) = -1 */
    }

    /* Search odd primes starting from 3 */
    uint64_t p = 3;
    for (;;)
    {
        /* Compute (-n) mod p */
        uint64_t a_mod = (p - (u64 % p)) % p;
        int leg = legendre_symbol(a_mod, p);
        if (leg == -1)
            return (seq_t)p;                     /* found smallest prime */

        /* Next prime > p */
        p = next_prime(p + 1);
    }
}
