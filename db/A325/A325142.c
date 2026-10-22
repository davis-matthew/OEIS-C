/*
 * A325142  a(n) = k if (n - k, n + k) is the centered Goldbach partition of
 *          2n if it exists and -1 otherwise.
 * https://oeis.org/A325142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1735, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute centered Goldbach partition offset by brute‑force search */
#include <stdbool.h>

/* Simple trial division primality test for uint64_t values */
static bool is_prime_u64(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2)            /* no partition for 0 or 1 */
        return (seq_t)-1;

    for (idx_t k = 0; ; ++k)
    {
        idx_t p = n - k;
        if (p < 2)       /* primes are >=2, no need to continue */
            break;
        if (is_prime_u64(p) && is_prime_u64(n + k))
            return (seq_t)k;   /* minimal k found */
    }
    return (seq_t)-1;           /* no centered Goldbach partition */
}
