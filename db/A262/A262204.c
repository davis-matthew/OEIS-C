/*
 * A262204  a(n) = (2*prime(n))! / prime(n)!.
 * https://oeis.org/A262204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*prime(n))! / prime(n)! (product of the integers
 *            prime(n)+1 … 2*prime(n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;  /* Undefined for n=0, offset starts at 1 */
    }

    idx_t p = get_prime(n);          /* n‑th prime (1‑indexed) */
    seq_t result = 1;

    /* multiply (p+1) * (p+2) * … * (2p) */
    for (idx_t i = 1; i <= p; ++i) {
        result *= ((seq_t)p + i);
    }

    return result;
}
