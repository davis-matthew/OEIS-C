/*
 * A363332  a(n) is the number of divisors of n that are both coreful and
 *          bi-unitary.
 * https://oeis.org/A363332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function with a(p^e) = e - 1 + (e mod 2) */
/* a(n) = product over prime powers p^e || n of (e if e odd else e-1) */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;                /* a(1) = 1 */

    idx_t temp = n;
    seq_t result = 1;

    /* factor n by trial division */
    for (idx_t p = 2; p <= temp / p; /* increment inside */) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            result *= (exp & 1) ? (seq_t)exp : (seq_t)(exp - 1);
        }
        /* advance to the next candidate divisor */
        if (p == 2) p = 3;
        else p += 2;
    }

    /* if a prime factor > sqrt(n) remains */
    if (temp > 1) {
        /* remaining prime has exponent 1 (odd) → multiplier 1 */
        /* result *= 1;  // no effect */
    }

    return result;
}
