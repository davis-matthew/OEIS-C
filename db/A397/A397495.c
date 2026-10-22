/*
 * A397495  Number of quadruples (a,b,c,d) of nonzero quadratic residues
 *          modulo prime(n) such that a + b == c + d (mod prime(n)).
 * https://oeis.org/A397495
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p-1)*(p^2-2p+9)/16 if p ≡ 1 (mod 4) a(n) =
 *            (p-1)*(p^2-2p+5)/16 if p ≡ 3 (mod 4) where p = prime(n) (the nth
 *            prime, n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 297, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Ensure we are called only for n ≥ 2; otherwise return 0 */
    if (n < 2) return (seq_t)0;

    uint32_t p_uint = get_prime(n);          /* p fits in 32 bits */
    seq_t   p        = (seq_t)p_uint;       /* promote to seq_t */
    seq_t   p_minus1 = p - (seq_t)1;

    /* Compute the appropriate term depending on p mod 4 */
    seq_t term;
    if ((p_uint & 3U) == 1U) {              /* p ≡ 1 (mod 4) */
        term = p * p - (seq_t)2 * p + (seq_t)9; /* p^2 - 2p + 9 */
    } else {                                /* p ≡ 3 (mod 4) */
        term = p * p - (seq_t)2 * p + (seq_t)5; /* p^2 - 2p + 5 */
    }

    seq_t numerator = p_minus1 * term;
    seq_t result    = numerator / (seq_t)16;
    return result;
}
