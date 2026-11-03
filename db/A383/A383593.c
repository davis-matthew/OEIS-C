/*
 * A383593  In the binary expansion of n, change the most significant 0 bit to
 *          1, if there is any 0 bit.
 * https://oeis.org/A383593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n + floor(2^(A063250(n)-1)) for n>0; a(0)=1 (most
 *            significant 0 bit set to 1)
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 85 terms through n = 9523 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* Find the position of the most significant 0 bit in n.
       bitlen is the number of bits in the binary representation of n. */
    idx_t bitlen = ilog2(n) + 1;
    for (idx_t i = bitlen; i-- > 0; ) {            /* i goes from bitlen-1 down to 0 */
        if (((n >> i) & 1ULL) == 0ULL) {           /* if this bit is 0 */
            return (seq_t)n + ((seq_t)1 << i);    /* set it to 1 */
        }
    }
    /* n has no 0 bits in its binary representation; return n itself */
    return (seq_t)n;
}
