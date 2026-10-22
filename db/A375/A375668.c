/*
 * A375668  The maximum exponent in the prime factorization of the 7-rough
 *          numbers (A007775).
 * https://oeis.org/A375668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorization of the nth
 *            7‑rough number (numbers whose prime factors are all ≥ 7)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the n‑th 7‑rough number and return its largest prime exponent. */
seq_t a(idx_t n)
{
    /* Special case: for n < 1 no well‑defined answer – return 0 */
    if (n == 0) return (seq_t)0;

    /* Find the n‑th 7‑rough number */
    idx_t found = 0;
    uint64_t rough = 0;
    for (uint64_t x = 1; ; ++x) {
        if ((x & 1u) && (x % 3u) && (x % 5u)) { /* not divisible by 2,3,5 */
            ++found;
            if (found == n) {
                rough = x;
                break;
            }
        }
    }

    /* Compute the largest exponent in the prime factorization of `rough`. */
    seq_t max_exp = 0;
    uint64_t m = rough;

    /* Trial division from 7 upwards (composite divisors are harmless here). */
    for (uint64_t p = 7; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp > max_exp) max_exp = exp;
        }
    }

    /* If there is a remaining prime factor > 1, its exponent is 1 */
    if (m > 1 && max_exp < 1) max_exp = 1;

    return max_exp;
}
