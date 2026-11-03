/*
 * A366973  Smallest odd prime p such that n^((p+1)/2) == n (mod p).
 * https://oeis.org/A366973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime p such that n^((p+1)/2) ≡ n (mod p)
 *            (i.e. Legendre symbol (n|p) ≥ 0). Computed by a simple search
 *            over odd primes and checking the Jacobi symbol
 * Range:     u8 n <= 1486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9119 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

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
    /* Convert index to seq_t once, to avoid re-creating each loop */
    seq_t value = (seq_t)n;

    /* Search over odd primes starting from 3 */
    for (uint64_t p = 3; ; p += 2)
    {
        if (!is_prime_seq((seq_t)p))
            continue;                  /* skip composites */

        seq_t mod = value % (seq_t)p;

        /* If n is 0 mod p, the condition holds trivially */
        if (mod == 0)
            return (seq_t)p;

        /* Compute Jacobi symbol (mod | p); for prime p this equals
           the Legendre symbol.  A value of 1 means n is a quadratic
           residue mod p. */
        int j = jacobi_symbol((uint64_t)mod, (uint64_t)p);
        if (j == 1)
            return (seq_t)p;
    }
}
