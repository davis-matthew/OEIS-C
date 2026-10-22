/*
 * A340714  a(n) is the sum of (n-2*j) for j < n/2 coprime to n.
 * https://oeis.org/A340714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} [gcd(n,k)=1] * (n-2k)
 * Range:     u8 n <= 36, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (The indicator [gcd(n,k)=1] is 1 if gcd(n,k)=1 and 0 otherwise.) */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sum = (seq_t)0;
    if (n < 3) return sum;               /* a(1)=0, a(2)=0 */
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k <= (n - 1) / 2; ++k) {
        if (gcd(sn, (seq_t)k) == (seq_t)1) {
            sum += sn - (seq_t)2 * (seq_t)k;
        }
    }
    return sum;
}
