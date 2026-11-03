/*
 * A359465  a(n) = 1 if n is an odd squarefree number with an even number of
 *          prime factors, otherwise 0.
 * https://oeis.org/A359465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is an odd squarefree integer with an even number
 *            of distinct prime factors, otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9954 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Test whether x is square‑free.  Simple trial division works
   because numbers in the sequence are relatively small for the
   required range of n. */
static bool is_squarefree(seq_t x)
{
    for (idx_t d = 2; (seq_t)d * (seq_t)d <= x; ++d) {
        if (x % d == 0) {
            x /= d;
            if (x % d == 0) return false;
        }
    }
    return true;
}

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

seq_t a(idx_t n)
{
    if ((n & 1) == 0) /* even n */
        return (seq_t)0;

    seq_t x = (seq_t)n;
    if (!is_squarefree(x))
        return (seq_t)0;

    int omega = count_distinct_primes(n);
    return (seq_t)((omega & 1) == 0 ? 1 : 0);
}
