/*
 * A283318  Image of 0 under repeated applications of the morphism 0 ->
 *          0,1,0,0, 1 -> 1,1,0,1.
 * https://oeis.org/A283318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(4n) = a(4n-3); a(4n-1)=0; a(4n-2)=1; Initial terms:
 *            a(1)=0, a(2)=1, a(3)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) {            /* handle the three small indexes directly */
        switch ((int)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
            case 3: return (seq_t)0;
            default: /* n==0 – not expected, but keep type safety */ return (seq_t)0;
        }
    }
    /* reduce the index using the morphism rules */
    switch ((int)(n & 3)) {
        case 0:  /* n ≡ 0 (mod 4) : a(n) = a(n/4)               */
            return a(n >> 2);
        case 1:  /* n ≡ 1 (mod 4) : a(n) = a((n+3)/4) = a((n>>2)+1) */
            return a((n >> 2) + 1);
        case 2:  /* n ≡ 2 (mod 4) : a(n) = 1                      */
            return (seq_t)1;
        case 3:  /* n ≡ 3 (mod 4) : a(n) = 0                      */
            return (seq_t)0;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
