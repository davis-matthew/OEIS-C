/*
 * A354901  a(n) = (b(2n) - 1)/2 - n for n > 0. To get b(n) start with A = n
 *          and then for i = 0..f(n) apply A := A + 2^i*T(A, f(n) - i) where
 *          T(n, k) = floor(n/2^k) mod 2 and f(n) = A000523(n).
 * https://oeis.org/A354901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(2n) - 1)/2 - n where b(n) starts with A = n and then
 *            for i = 0..f(n) apply A := A + 2^i * T(A, f(n) - i) with T(n, k)
 *            = floor(n/2^k) mod 2 and f(n) = floor(log2(n)) (number of bits
 *            of n minus 1)
 * Range:     u8 n <= 63, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"  /* bitlen_seq_t(seq_t)          */
#include "../utils/pow2.h"          /* pow2(idx_t exp)              */

static seq_t b_of_seq(seq_t n)
{
    /* In the OEIS definition f(n) is the binary length minus 1. */
    unsigned int f = bitlen_seq_t(n);
    if (f == 0)            /* n == 0 would not occur for the sequence      */
        return n;          /* but guard against it for complete              */
    f -= 1;                 /* now f = floor(log2(n))                         */

    seq_t A = n;
    for (unsigned int i = 0; i <= f; ++i)
    {
        seq_t bit = (A >> (f - i)) & (seq_t)1;   /* T(A, f-i)                 */
        if (bit)
            A += pow2((idx_t)i);                /* add 2^i if the bit is 1   */
    }
    return A;
}

seq_t a(idx_t n)
{
    /* b(2n) is needed – compute 2n in seq_t to avoid overflow of idx_t */
    seq_t m = ((seq_t)n) << 1;          /* m = 2n                      */
    seq_t bm = b_of_seq(m);              /* b(2n)                       */
    seq_t res = (bm - 1) / 2 - (seq_t)n; /* a(n) = (b(2n)-1)/2 - n      */
    return res;
}
