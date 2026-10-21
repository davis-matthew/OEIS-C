/*
 * A244799  Number of moduli m such that (prime(n) mod m) is odd, where 1 <= m
 *          < prime(n).
 * https://oeis.org/A244799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count m such that 1 <= m < prime(n) and (prime(n) mod m) is odd. */
/* Even m always give an odd residue.  For odd m the residue is odd
   iff floor(prime(n)/m) is even. */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Retrieve the nth prime.  get_prime returns a uint32_t which is
       safe for n up to the size of the b-file (primes fit in 32 bits). */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* All even m give odd residues. */
    seq_t even_m = (p - 1) / 2;

    /* Count odd m with floor(p/m) even. */
    seq_t odd_even_k = 0;
    for (uint32_t m = 3; m < p32; m += 2) {
        uint32_t k = p32 / m; /* floor division */
        if ((k & 1u) == 0u)    /* even k */
            odd_even_k++;
    }

    return even_m + odd_even_k;
}
