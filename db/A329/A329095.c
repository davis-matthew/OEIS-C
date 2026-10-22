/*
 * A329095  Odd numbers k such that x^2 == 2 (mod k) has no solution.
 * https://oeis.org/A329095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth odd number k such that x^2 == 2 (mod k) has no
 *            solution
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_prime_congr3or5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_prime_congr3or5.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, but guard against n=0 */

    idx_t  count = 0;
    uint64_t k = 3;              /* odd numbers start at 3 */

    for (;; k += 2) {
        if (has_prime_congr3or5(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
