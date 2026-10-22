/*
 * A365702  G.f. A(x) satisfies A(x) = 1 + x^5*A(x)^5 / (1 - x*A(x)).
 * https://oeis.org/A365702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 37, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binom_compute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A365702
   Formula: a(n) = (1/(n+1)) * Sum_{k=0..⌊n/5⌋} C(n-4k-1, n-5k) * C(n+1,k) */
#include <stdbool.h>
#include "../utils/binom_compute.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) = 1 */
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 5;           /* ⌊n/5⌋ */
    for (idx_t k = 1; k <= maxk; ++k) {   /* k = 0 gives zero contribution */
        idx_t n1 = n - 4 * k - 1;   /* first binomial top argument */
        idx_t k1 = n - 5 * k;       /* first binomial bottom argument */
        seq_t b1 = binom_compute(n1, k1);   /* C(n1,k1) */
        seq_t b2 = binom_compute(n + 1, k); /* C(n+1,k)   */
        sum = sum + b1 * b2;
    }
    return sum / (seq_t)(n + 1);   /* divide by n+1 */
}
