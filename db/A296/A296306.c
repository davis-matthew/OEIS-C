/*
 * A296306  a(n) = A001157(n)/A050999(n).
 * https://oeis.org/A296306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = (4^(v_2(n)+1)-1)/3
 * Range:     u8 n <= 7, u16 n <= 255, u32 n <= 9195, u64 n <= 9195,
 *            u128 n <= 9195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* multiplicative: a(2^e * m) = (4^(e+1)-1)/3,   a(odd) = 1 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, but guard 0 */

    /* count trailing zeros of n  (i.e. exponent of 2 in n) */
    unsigned e = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        ++e;
        tmp >>= 1;
    }

    /* compute 4^(e+1) */
    seq_t pow4 = (seq_t)1;
    for (unsigned i = 0; i < e + 1; ++i)
        pow4 *= (seq_t)4;

    /* a(n) = (4^(e+1)-1)/3 */
    return (pow4 - (seq_t)1) / (seq_t)3;
}
