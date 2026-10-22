/*
 * A366763  The number of divisors of n that have no exponent 2 in their prime
 *          factorization.
 * https://oeis.org/A366763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} max(e,2) (product over the prime powers of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Computes A366763(n): the number of divisors of n that have no exponent 2
 * in their prime‑factorisation.  Valid for all n ≥ 1.  If n == 0 we return 0
 * as a defensive default. */
seq_t a(idx_t n)
{
    /* Treat the non‑positive case defensively. */
    if (n == 0) return 0;
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;     /* use 64‑bit arithmetic for factorisation */
    uint64_t exp;

    /* Handle factor 2 separately for speed. */
    if (m % 2 == 0) {
        exp = 0;
        while (m % 2 == 0) {
            ++exp;
            m /= 2;
        }
        result *= (seq_t)(exp >= 2 ? exp : 2);
    }

    /* Trial division for odd primes. */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            result *= (seq_t)(exp >= 2 ? exp : 2);
        }
    }

    /* If remaining m > 1, it is a prime factor with exponent 1. */
    if (m > 1) {
        result *= (seq_t)2;   /* max(1,2) = 2 */
    }

    return result;
}
