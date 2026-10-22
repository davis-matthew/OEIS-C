/*
 * A302295  a(n) is the period of the binary expansion of n (with leading
 *          zeros allowed).
 * https://oeis.org/A302295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the least k such that n is a repdigit base 2^k (digits of
 *            n in base 2^k are all equal)
 * Range:     u8 n <= 552, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    /* bitlen = floor(log2(n)) + 1 */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) { tmp >>= 1; ++bitlen; }
    if (bitlen == 0) return 1;    /* should not happen for n>0 */

    seq_t base = 2;                /* 2^1 */
    for (idx_t k = 1; k < bitlen; ++k) {
        seq_t d = (seq_t)n % base;
        seq_t t = (seq_t)n;
        int all_same = 1;
        while (t > 0) {
            if ((t % base) != d) { all_same = 0; break; }
            t /= base;
        }
        if (all_same) return (seq_t)k;
        base <<= 1;               /* prepare base = 2^(k+1) for next iteration */
    }

    return (seq_t)bitlen;          /* base > n, single digit repdigit */
}
