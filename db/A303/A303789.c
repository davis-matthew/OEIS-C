/*
 * A303789  a(n) = Sum_{i=0..m} d(i)*6^i, where Sum_{i=0..m} d(i)*7^i is the
 *          base-7 representation of n.
 * https://oeis.org/A303789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} d_i * 6^i, where n = Σ_{i≥0} d_i * 7^i (d_i are
 *            the base‑7 digits of n)
 * Range:     u8 n <= 339, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;          /* power = 6^i at each iteration */
    while (n > 0) {
        seq_t digit = (seq_t)(n % 7);   /* base‑7 digit */
        result += digit * power;        /* add d_i * 6^i */
        n /= 7;                        /* move to next digit */
        power *= 6;                     /* advance 6^i -> 6^(i+1) */
    }
    return result;
}
