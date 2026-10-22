/*
 * A347693  Smallest k >= 2*n such that k is a prime or twice a prime.
 * https://oeis.org/A347693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k >= 2*n such that k is prime or twice a prime
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t cand = 2 * n;
    while (1) {
        if (is_prime(cand) || ((cand & 1) == 0 && is_prime(cand >> 1))) {
            return (seq_t)cand;
        }
        cand++;
    }
}
