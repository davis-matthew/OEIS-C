/*
 * A382393  Positive integers k such that 6*k - 1 is prime for k != 1 (mod 5)
 *          and (6*k - 1)/5 is prime for k == 1 (mod 5).
 * https://oeis.org/A382393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates integers k such that
   6*k-1 is prime for k ≠ 1 (mod 5), and (6*k-1)/5 is prime for k = 1 (mod 5). */

#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 1; ; ++k) {
        bool good = false;
        if (k % 5 == 1) {
            seq_t m = ((k * 6) - 1) / 5;
            good = is_prime_big(m);
        } else {
            seq_t m = (k * 6) - 1;
            good = is_prime_big(m);
        }
        if (good) {
            ++count;
            if (count == n) {
                return k;
            }
        }
    }
}
