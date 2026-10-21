/*
 * A273163  P-defects p - N(p) of the congruence y^2 == x^3 + x^2 + 4*x + 4
 *          (mod p) for primes p, where N(p) is the number of solutions given
 *          for p = prime(n) by A272207(n).
 * https://oeis.org/A273163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 613, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  OEIS A273163 – P‑defects of the elliptic curve
 *
 *  a(n) = prime(n) – N(prime(n))
 *  where N(p) is the number of solutions (x,y)∈F_p² of
 *      y^2 ≡ x^3 + x^2 + 4x + 4 (mod p)
 *
 *  The implementation counts the solutions by brute force:
 *      for each x∈{0,…,p−1} compute RHS = (x^3+x^2+4x+4) mod p.
 *      If RHS=0  → one solution for y.
 *      If RHS≠0  → use the Legendre symbol (jacobi_symbol) to test
 *                 whether RHS is a quadratic residue; if so,
 *                 two solutions for y, otherwise none.
 *
 *  Complexity:  O(p) arithmetic operations per call.
 *
 *********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

/* Counts the number of solutions of y^2 = x^3 + x^2 + 4x + 4 (mod p) */
static seq_t
count_solutions(uint64_t p)
{
    seq_t count = 0;
    for (uint64_t x = 0; x < p; ++x) {
        /* Compute RHS modulo p */
        uint64_t xx = (x * x) % p;          /* x^2 */
        uint64_t x3 = (xx * x) % p;         /* x^3 */
        uint64_t rhs = (x3 + xx) % p;       /* x^3 + x^2 */
        rhs = (rhs + (4 * x) % p) % p;      /* + 4x */
        rhs = (rhs + 4) % p;                /* + 4 */

        if (rhs == 0) {
            count += 1;                    /* y ≡ 0 is the only solution */
        } else {
            int ls = jacobi_symbol(rhs, p); /* Legendre symbol */
            if (ls == 1) {
                count += 2;                /* two square roots exist */
            }
        }
    }
    return count;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* prime(n) – count_solutions(prime(n)) */
    uint64_t p = get_prime(n);
    seq_t Np = count_solutions(p);
    return (seq_t)p - Np;
}
