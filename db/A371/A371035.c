/*
 * A371035  a(n) = A086330(prime(n)).
 * https://oeis.org/A371035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A086330(prime(n)) where A086330(p) = Σ_{k=2}^{p-1} (k!
 *            mod p)
 * Range:     u8 n <= 9, u16 n <= 73, u32 n <= 8945, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* out of bounds, but undefined behaviour */
    /* Get the n-th prime.  It fits in 32 bits for the intended ranges. */
    uint64_t p = get_prime(n);

    seq_t sum  = 0;
    seq_t fact = 1;                          /* 1! */
    idx_t  lim  = (idx_t)p;                  /* loop up to p-1 */

    for (idx_t k = 2; k < lim; ++k) {
        /* compute next factorial mod p, using 64‑bit temp to avoid overflow */
        uint64_t prod = (uint64_t)fact * (uint64_t)k;
        fact = (seq_t)(prod % p);
        sum += fact;
    }
    return sum;
}
