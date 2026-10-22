/*
 * A343251  a(n) is the least k0 <= n such that v_5(n), the 5-adic order of n,
 *          can be obtained by the formula: v_5(n) = log_5(n / L_5(k0, n)),
 *          where L_5(k0, n) is the lowest common denominator of the elements
 *          of the set S_5(k0, n) = {(1/n)*binomial(n, k), with 0 < k <= k0
 *          such that k is not divisible by 5} o...
 * https://oeis.org/A343251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greatest prime power dividing n that is not a power
 *            of 5; if n has no such prime factor, a(n)=1
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    seq_t maxPow = (seq_t)1;
    idx_t temp = n;
    for (idx_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            seq_t power = 1;
            while (temp % i == 0) {
                temp /= i;
                power = power * (seq_t)i;
            }
            if (i != 5 && power > maxPow) {
                maxPow = power;
            }
        }
    }
    if (temp > 1) {
        seq_t power = (seq_t)temp;
        if (temp != 5 && power > maxPow) {
            maxPow = power;
        }
    }
    return maxPow;
}
