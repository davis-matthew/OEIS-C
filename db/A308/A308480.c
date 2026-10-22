/*
 * A308480  a(n) = A000225(n) if n is prime, a(n) = A020639(n) otherwise.
 * https://oeis.org/A308480
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n-1 if n is prime; otherwise a(n) = smallest prime
 *            factor of n
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the smallest prime factor of n; if n is prime, returns n itself. */
static uint64_t spf_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL)
        return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL)
            return d;
    }
    return n;            /* n is prime */
}

/* Compute 2^exp as a seq_t value (fast exponentiation). */
static seq_t power_of_two(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp > 0) {
        if (exp & 1ULL)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2ULL)      /* outside the defined offset; undefined behaviour not required */
        return (seq_t)0;

    uint64_t spf = spf_uint64((uint64_t)n);
    if (spf == (uint64_t)n) {          /* n is prime */
        seq_t pow2 = power_of_two(n);
        return pow2 - (seq_t)1;        /* 2^n - 1 */
    } else {                           /* composite */
        return (seq_t)spf;
    }
}
