/*
 * A344509  a(n) = (1/n) * Sum_{k=1..n} k * lcm(k,n).
 * https://oeis.org/A344509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} k * lcm(k, n)
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;          /* k² */
        seq_t g  = gcd((seq_t)k, (seq_t)n);      /* gcd(k, n) */
        sum += kk / g;                           /* k² / gcd(k, n) */
    }
    return sum;                                  /* a(n) = Σ k² / gcd(k, n) */
}
