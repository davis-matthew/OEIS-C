/*
 * A392240  Largest prime factor of 6*n+1.
 * https://oeis.org/A392240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392240: a(n) = largest prime factor of 6*n+1 */
#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* Compute 6*n+1 */
    seq_t val = (seq_t)n * 6 + 1;

    /* If the value fits in 64‑bit, use the fast helper. */
    if (val <= (seq_t)UINT64_MAX) {
        uint64_t v = (uint64_t)val;
        return (seq_t)largest_prime_divisor(v);
    }

    /* Generic trial‑division for larger values. */
    seq_t largest = 0;
    seq_t temp = val;
    seq_t i = 2;

    while (i * i <= temp) {
        if (temp % i == 0) {
            largest = i;
            while (temp % i == 0) {
                temp /= i;
            }
        }
        i++;
    }
    if (temp > 1) {
        largest = temp;
    }
    return largest;
}
