/*
 * A390809  The sum of the integers k from 1 to n such that gcd(n, k) is
 *          either 1 or 2.
 * https://oeis.org/A390809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n, gcd(k,n) <= 2} k
 * Range:     u8 n <= 25, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"  /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        if (gcd(k_seq, n_seq) <= (seq_t)2) {
            total += k_seq;
        }
    }
    return total;
}
