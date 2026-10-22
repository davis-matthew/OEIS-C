/*
 * A343513  a(n) = Sum_{k=1..n} (k/gcd(n, k))^3.
 * https://oeis.org/A343513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k/gcd(n, k))^3
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 366, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"  /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n) {
    seq_t sum = 0;            /* accumulator */
    seq_t n_seq = (seq_t)n;   /* convert index to seq_t once */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t g = gcd(k_seq, n_seq);          /* gcd(n, k) */
        seq_t t = k_seq / g;                  /* k/gcd(n, k) */
        sum += t * t * t;                     /* add (k/g)^3 */
    }

    return sum;
}
