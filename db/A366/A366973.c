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
 * Helpers:   is_prime_small, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t once, to avoid re-creating each loop */
    seq_t value = (seq_t)n;

    /* Search over odd primes starting from 3 */
    for (uint64_t p = 3; ; p += 2)
    {
        if (!is_prime_small((seq_t)p))
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
