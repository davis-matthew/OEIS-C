/*
 * A277068  a(n) = gcd(s1, s2), where s1 is the sum of the odd numbers and s2
 *          is the sum of the even numbers in the Collatz (3x+1)trajectory of
 *          n.
 * https://oeis.org/A277068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sum of odd terms, sum of even terms) in the Collatz
 *            trajectory of n
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t s1 = 0, s2 = 0;
    seq_t cur = (seq_t)n;
    while (1) {
        if (cur & 1) s1 += cur;
        else          s2 += cur;
        if (cur == 1) break;
        if (cur & 1)  cur = cur * 3 + 1;
        else          cur >>= 1;
    }
    return gcd(s1, s2);
}
