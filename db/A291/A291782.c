/*
 * A291782  Let f_k(n) be the result of applying phi (the Euler totient
 *          function A000010) k times to n; a(n) = n*Product_{k=1..oo} f_k(n).
 * https://oeis.org/A291782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n * φ(n) * φ(φ(n)) * … until 1 is reached
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 136, u64 n <= 1096,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n;   /* start with n itself */
    seq_t v    = (seq_t)n;   /* current totient value */

    while (v > 1) {
        v = phi((idx_t)v);   /* φ(v) */
        prod *= v;           /* multiply this totient in the product */
    }
    return prod;
}
