/*
 * A244663  Binary representation of 4^n + 2^(n+1) - 1.
 * https://oeis.org/A244663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 4^n + 2^(n+1) - 1 represented as a decimal number whose
 *            digits are the bits of that integer in binary
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   bitlen_seq_t, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_int.h"   /* pow_int(seq_t base, idx_t exp) */
#include "../utils/bitlen_seq_t.h"  /* bitlen_seq_t(seq_t x) */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1.  For n==0 we return 0 (undefined). */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute 4^n + 2^(n+1) - 1 as a binary value. */
    seq_t term4 = pow_int((seq_t)2, 2 * n);        /* 4^n = 2^(2n)   */
    seq_t term2 = pow_int((seq_t)2, n + 1);        /* 2^(n+1)       */
    seq_t value = term4 + term2 - (seq_t)1;        /* 4^n + 2^(n+1) - 1 */

    /* Convert the binary representation to a decimal integer whose
       digits are the bits (0 or 1) of 'value'. */
    unsigned bitlen = bitlen_seq_t(value);
    seq_t result = (seq_t)0;

    for (int i = (int)bitlen - 1; i >= 0; --i) {
        seq_t bit = (value >> i) & ((seq_t)1);
        result = result * (seq_t)10 + bit;
    }

    return result;
}
