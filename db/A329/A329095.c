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
 * Validated: standard, pass, 78 terms through n = 9657 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether k has at least one odd prime factor congruent to 3 or 5 modulo 8. */
static bool has_prime_congr3or5(uint64_t k)
{
    uint64_t temp = k;
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if ((p % 8 == 3) || (p % 8 == 5))
                return true;
            while (temp % p == 0)
                temp /= p;
        }
    }
    /* Remaining factor > 1 is prime. */
    if (temp > 1) {
        if ((temp % 8 == 3) || (temp % 8 == 5))
            return true;
    }
    return false;
}

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
