/*
 * A352526  a(n) = Product_{k=0..n} Nimsum (2*k + 2), with Nimsum (2 + 2) = 0
 *          replaced by 1.
 * https://oeis.org/A352526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * a(n-1) * (n + (-1)^n) for n > 1 (with a(0) = 2, a(1)
 *            = 2). Equivalent to the product definition in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)2;
    }

    seq_t res = (seq_t)2;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* Factor = 2 * (i + (-1)^i) */
        seq_t factor = (seq_t)2 * ((seq_t)i + ((i & 1) ? (seq_t)-1 : (seq_t)1));
        res = res * factor;
    }
    return res;
}
