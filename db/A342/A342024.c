/*
 * A342024  a(n) = 1 if prime(k)^(k+1) divides n for some k, otherwise 0.
 * https://oeis.org/A342024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃k such that prime(k)^(k+1) divides n, otherwise 0
 * Range:     u8 n <= 40, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* List of the first 11 primes (k = 1..11). */
    static const idx_t primes[11] = {2,3,5,7,11,13,17,19,23,29,31};
    /* Pre‑computed values of prime(k)^(k+1). */
    static seq_t powers[11] = {0};
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i < 11; ++i) {
            seq_t res = 1;
            /* exponent = (k+1) = i+2 */
            for (idx_t j = 0; j < i + 2; ++j)
                res *= primes[i];
            powers[i] = res;
        }
        initialized = true;
    }

    /* n < 1 is outside the offset; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;

    for (idx_t i = 0; i < 11; ++i) {
        seq_t p = powers[i];
        /* If p > nn, the remainder will be nn ≠ 0; skip checks. */
        if ((nn % p) == 0)
            return (seq_t)1;
    }
    return (seq_t)0;
}
