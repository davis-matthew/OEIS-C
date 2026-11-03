/*
 * A284570  a(n) = A000005((n+1)^2) - A000005(n^2).
 * https://oeis.org/A284570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000005((n+1)^2) - A000005(n^2)
 * Range:     i8 n <= 418, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9983 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* count the number of divisors of n², i.e. of n squared */
static seq_t tau_square(uint64_t n)
{
    seq_t result = 1;

    /* factor 2 separately */
    uint64_t exp = 0;
    while ((n & 1ULL) == 0) {
        exp++;
        n >>= 1;
    }
    if (exp > 0) {
        result *= (seq_t)(2 * exp + 1);
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        exp = 0;
        while (n % p == 0) {
            exp++;
            n /= p;
        }
        if (exp > 0) {
            result *= (seq_t)(2 * exp + 1);
        }
    }

    /* remaining prime factor, if any */
    if (n > 1) {
        /* exponent 1 -> 2*1+1 = 3 */
        result *= 3;
    }

    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    uint64_t nn = (uint64_t)n;
    seq_t d_n   = tau_square(nn);
    seq_t d_np1 = tau_square(nn + 1);
    return d_np1 - d_n;
}
