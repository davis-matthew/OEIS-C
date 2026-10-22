/*
 * A341284  a(n) is the least prime == -prime(n) (mod 2*prime(n+1)).
 * https://oeis.org/A341284
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least odd prime p such that p ≡ -prime(n) (mod
 *            2*prime(n+1))
 * Range:     u8 n <= 15, u16 n <= 337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t pk  = (seq_t)get_prime(n);
    seq_t pk1 = (seq_t)get_prime(n + 1);
    seq_t m   = pk1 * ((seq_t)2u);     /* modulus 2*prime(n+1) */
    seq_t r   = m - pk;                /* residue modulo m */
    seq_t p   = r;
    while (!isPrimeGeneric(p)) {       /* find the least odd prime in the residue class */
        p += m;
    }
    return p;
}
