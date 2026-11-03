/*
 * A376504  Number of divisors of n that are both composite and squarefree.
 * https://oeis.org/A376504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^omega(n) - omega(n) - 1 * where omega(n) is the number
 *            of distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9738 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥1 */
    uint64_t nn = (uint64_t)n;

    /* Compute ω(n) – the number of distinct prime factors. */
    size_t omega = count_distinct_primes(nn);

    /* For ω(n) = 0 or 1 the result is 0, so we can return early. */
    if (omega <= 1)
        return (seq_t)0;

    /* 2^ω(n) */
    seq_t pow2omega = pow2((idx_t)omega);

    /* a(n) = 2^ω(n) - ω(n) - 1 */
    seq_t result = pow2omega - (seq_t)omega - (seq_t)1;
    return result;
}
