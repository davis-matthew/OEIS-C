/*
 * A247014  Number of binary centrosymmetric matrices of size n X n.
 * https://oeis.org/A247014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(A000982(n)) where A000982(n) = n^2/2 for even n, *
 *            A000982(n) = (n^2 + 1)/2 for odd n. Equivalently * a(n) =
 *            2^((n*n)/2 + (n & 1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute the exponent e = floor(n^2/2) + (n odd ? 1 : 0). */
    idx_t prod = n * n;
    idx_t exp  = prod / 2;
    if (n & 1U)  /* n is odd */
        exp += 1U;
    return pow_int((seq_t)2, exp);
}
