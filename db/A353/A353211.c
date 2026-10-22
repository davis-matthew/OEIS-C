/*
 * A353211  a(n) is the number of diagonalizable 2 X 2 matrices over
 *          GF(prime(n)).
 * https://oeis.org/A353211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)^4 - prime(n)^2 + 2*prime(n))/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based index; prime(n) returns the n‑th prime as uint32_t */
    uint32_t p32 = get_prime(n);
    seq_t p   = (seq_t)p32;
    seq_t p2  = p * p;          /* p^2 */
    seq_t p4  = p2 * p2;        /* p^4 */
    seq_t num = p4 - p2 + 2 * p;
    return num / 2;
}
