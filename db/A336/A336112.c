/*
 * A336112  a(n) is the least number k such that the Sum_{i=0..k} sqrt(k)
 *          equals or exceeds n.
 * https://oeis.org/A336112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that Σ_{i=0..k} sqrt(i) ≥ n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    long double target = (long double)n;
    long double sum = 0.0L;
    idx_t k = 0;
    for (;; ++k) {
        sum += sqrtl((long double)k);
        if (sum >= target) return (seq_t)k;
    }
}
