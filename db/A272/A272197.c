/*
 * A272197  Number of solutions of the congruence y^2 == x^3 + 1 (mod p) as p
 *          runs through the primes.
 * https://oeis.org/A272197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions of y^2 == x^3 + 1 (mod prime(n))
 *            Implementation: loop over all x in F_p, compute rhs = x^3+1 mod
 *            p, add (legendre_symbol(rhs, p) + 1) to the total count
 * Range:     u8 n <= 54, u16 n <= 6501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) corresponds to the first prime. */
    if (n == 0) return 0;  /* safety, though n ≥ 1 in valid calls */

    uint32_t p = get_prime(n);        /* prime(n), guaranteed > 0 */
    if (p == 2) return 2;             /* special case – see sequence example */

    seq_t count = 0;
    for (uint32_t x = 0; x < p; ++x) {
        /* Compute rhs = (x^3 + 1) mod p safely using 128‑bit arithmetic. */
        unsigned __int128 x_sq = (unsigned __int128)x * x % p;   /* x^2 mod p */
        unsigned __int128 x_cu = x_sq * x % p;                  /* x^3 mod p */
        uint64_t rhs = (uint64_t)((x_cu + 1) % p);              /* rhs < p */

        int ls = jacobi_symbol(rhs, p);   /* Legendre symbol (rhs/p), -1/0/1 */
        count += (seq_t)(ls + 1);          /* 0→1, 1→2, -1→0 solutions */
    }
    return count;
}
