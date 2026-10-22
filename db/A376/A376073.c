/*
 * A376073  Number of solutions of the congruence y^2 + y == x^3 - x^2 (mod p)
 *          as p runs through the primes.
 * https://oeis.org/A376073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376073: number of solutions of y^2 + y == x^3 - x^2 (mod p)
   where p is the nth prime (offset 1).  The value is computed by
   enumerating all x modulo p and determining the number of y that satisfy
   the quadratic congruence for each residue. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* obtain the nth prime; offset 1 means n = 1 gives prime 2 */
    seq_t p_seq = get_prime(n);
    uint64_t p = (uint64_t)p_seq;            /* p fits in 64 bits for the intended range   */

    /* special handling for p == 2 */
    if (p == 2) {
        seq_t count = 0;
        for (uint64_t x = 0; x < 2; ++x) {
            uint64_t rhs = (x * x + 2 - x * x) & 1; /* x^3 - x^2 mod 2 simplifies to 0 */
            if (rhs == 0) count += 2;               /* y = 0 or 1 satisfy the equation */
        }
        return count;
    }

    seq_t total = 0;
    for (idx_t ix = 0; ix < p; ++ix) {
        uint64_t x  = (uint64_t)ix;
        uint64_t xx = (unsigned __int128)x * x % p;          /* x^2 mod p   */
        uint64_t x3 = (unsigned __int128)xx * x % p;         /* x^3 mod p   */
        uint64_t rhs = (x3 + p - xx) % p;                   /* x^3 - x^2   */

        /* discriminant d = 1 + 4*rhs mod p */
        uint64_t d = ((unsigned __int128)4 * rhs % p + 1) % p;

        if (d == 0) {
            total += 1;         /* double root */
        } else {
            int leg = jacobi_symbol(d, p);  /* Legendre symbol for prime p */
            if (leg == 1) total += 2;        /* two distinct roots */
        }
    }
    return total;
}
