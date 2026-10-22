/*
 * A306652  a(n) = Sum_{m=1..n} Sum_{k=1..n} [k divides n]*[n/k divides m]*[k
 *          divides m + 4].
 * https://oeis.org/A306652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k|n} Σ_{j|4} [gcd(k, n/k) = j] * j * Approximate
 *            equivalent: for each divisor k of n, add gcd(k, n/k) if it * is
 *            1, 2, or 4. The divisor pairs (k, n/k) are processed once,
 *            counting * each divisor separately (so a square divisor
 *            contributes only once)
 * Range:     u8 n <= 9652, u16 n <= 9652, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   igcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/igcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j   = n / i;
            idx_t g   = igcd(i, j);

            if (g == 1 || g == 2 || g == 4)
                sum += (seq_t)g;

            if (i != j) {
                if (g == 1 || g == 2 || g == 4)
                    sum += (seq_t)g;   /* contribution for divisor j */
            }
        }
    }
    return sum;
}
