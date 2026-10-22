/*
 * A322977  Number of even divisors d of n such that d-1 is prime.
 * https://oeis.org/A322977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of even divisors d of n such that d-1 is prime
 * Range:     u8 n <= 617, u16 n <= 9828, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n <= 1) return 0;

    seq_t count = 0;
    idx_t s = 0;
    /* find floor sqrt(n) without overflow */
    while (true) {
        idx_t next = s + 1;
        if (next > n / next) break;
        s = next;
    }

    for (idx_t i = 1; i <= s; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 > 1 && (d1 & 1U) == 0) {
                if (is_prime_small((seq_t)(d1 - 1))) ++count;
            }
            if (d2 != d1 && d2 > 1 && (d2 & 1U) == 0) {
                if (is_prime_small((seq_t)(d2 - 1))) ++count;
            }
        }
    }
    return count;
}
