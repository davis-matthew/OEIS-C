/*
 * A244513  Legendre symbol (prime(n), prime(n+1)).
 * https://oeis.org/A244513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Legendre symbol of consecutive primes (prime(n) | prime(n+1))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Retrieve the nth and (n+1)th primes. */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Since p2 is prime, the Legendre symbol (p1 | p2) equals the
       Jacobi symbol, which is +1 or -1 for distinct primes. */
    int ls = jacobi_symbol((uint64_t)p1, (uint64_t)p2);

    return (seq_t)ls;
}
