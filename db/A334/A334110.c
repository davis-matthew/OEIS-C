/*
 * A334110  The squares of squarefree numbers (A062503), ordered
 *          lexicographically according to their prime factors. a(n) =
 *          Product_{k in I} prime(k+1)^2, where I are the indices of nonzero
 *          binary digits in n = Sum_{k in I} 2^k.
 * https://oeis.org/A334110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k∈I} prime(k+1)^2, where I are the indices of 1-bits
 *            in the binary representation of n. (So a(0)=1, a(1)=4, a(2)=9, …
 *            as in the OEIS sequence.)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t product = 1;
    idx_t t   = n;
    idx_t bit = 0;          /* current bit position (0‑based) */

    while (t != 0) {
        if (t & 1ULL) {
            /* prime(k+1) is the (bit+1)-th prime, since get_prime is 1‑indexed */
            product *= (seq_t)get_prime(bit + 1);
        }
        ++bit;
        t >>= 1;
    }

    return product * product;
}
