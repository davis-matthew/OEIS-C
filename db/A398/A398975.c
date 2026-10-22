/*
 * A398975  Lexicographically earliest sequence of positive integers such that
 *          a(n) equals the number of fixed points within distance a(n) of n.
 * https://oeis.org/A398975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(n) + (1-F(n))*(1 + 2*F(n-1)*F(n+1)), where F(n) = 1
 *            if popcount(n) is odd, except F(2)=0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity_of.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;

    unsigned fn;
    if (N == 2)
        fn = 0;
    else
        fn = parity_of(N);

    unsigned fm1;
    uint64_t m1 = N - 1;
    if (m1 == 2)
        fm1 = 0;
    else
        fm1 = parity_of(m1);

    unsigned fp1;
    uint64_t p1 = N + 1;
    if (p1 == 2)
        fp1 = 0;
    else
        fp1 = parity_of(p1);

    if (fn) {
        /* F(n) = 1  ⇒  a(n) = n */
        return (seq_t)N;
    }

    /* F(n) = 0  ⇒  a(n) = 1 + 2*F(n-1)*F(n+1) */
    seq_t res = 1;
    if (fm1 && fp1) res += 2;
    return res;
}
