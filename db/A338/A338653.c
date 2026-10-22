/*
 * A338653  Number of divisors of n which are greater than 9.
 * https://oeis.org/A338653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are > 9, i.e. a(n) = τ(n) –
 *            #{d | n , 1 ≤ d ≤ 9}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count total divisors τ(n). */
    seq_t tau = 1;
    idx_t temp = n;
    idx_t i = 2;
    while (i <= temp / i) {      /* i*i <= temp, without overflow */
        if (temp % i == 0) {
            idx_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
        ++i;
    }
    if (temp > 1) {
        tau *= (seq_t)2;            /* remaining prime factor */
    }

    /* Subtract divisors ≤ 9. */
    seq_t small = 0;
    for (idx_t d = 1; d <= 9; ++d) {
        if (n % d == 0) {
            ++small;
        }
    }

    return tau - small;
}
