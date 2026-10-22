/*
 * A332382  If n = Sum (2^e_k) then a(n) = Product (prime(e_k + 2)).
 * https://oeis.org/A332382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{e : bit e set in n} prime(e+2) (prime k = k‑th prime)
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition of OEIS A332382 directly. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t product = 1;          /* a(0) = 1 automatically */
    idx_t tmp = n;
    idx_t pos = 0;              /* bit position */

    while (tmp) {
        if (tmp & 1U) {
            /* multiply by the (pos+2)-th prime */
            product *= (seq_t)get_prime(pos + 2);
        }
        tmp >>= 1;
        pos++;
    }

    return product;
}
