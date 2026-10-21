/*
 * A278947  Expansion of Sum_{k>=1} (k*(5*k - 3)/2)*x^k/(1 - x^k).
 * https://oeis.org/A278947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d*(5*d-3)/2 (a(0)=0)
 * Range:     u8 n <= 9, u16 n <= 137, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    /* Integer square root of n (floor). */
    idx_t sqrt_n = (idx_t)sqrt((double)n);
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        sqrt_n++;
    while (sqrt_n * sqrt_n > n)
        sqrt_n--;

    for (idx_t i = 1; i <= sqrt_n; i++) {
        if (n % i == 0) {
            /* divisor i */
            seq_t d = (seq_t)i;
            seq_t term = (d * ((seq_t)i * 5 - 3)) >> 1;
            sum += term;

            /* paired divisor n/i, avoid double counting when i == n/i */
            idx_t j = n / i;
            if (j != i) {
                d = (seq_t)j;
                term = (d * ((seq_t)j * 5 - 3)) >> 1;
                sum += term;
            }
        }
    }
    return sum;
}
