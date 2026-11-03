/*
 * A307408  a(n) = (A001222(n) - 1)*A001221(n) + 2.
 * https://oeis.org/A307408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Ω(n) - 1) * ω(n) + 2
 * Range:     u8 n <= 13440, u16 n <= 13440, u32 n <= 13440, u64 n <= 13440,
 *            u128 n <= 13440, big any n
 * Validated: standard, pass, 120 terms through n = 9927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ω(n) = A001222(n) (number of prime factors of n, counted with multiplicity) */
/* ω(n) = A001221(n) (number of distinct prime factors of n) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t bigomega(idx_t n)
{
    /* Count prime factors of n with multiplicity. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2. */
    while ((n & 1) == 0) {
        n >>= 1;
        ++count;
    }

    /* Test odd divisors up to sqrt(n). */
    for (idx_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }

    /* If n > 1 now, it is a prime factor > sqrt(original n). */
    if (n > 1)
        ++count;

    return count;
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
    seq_t omega = bigomega(n);                 /* Ω(n) */
    seq_t distinct = (seq_t)count_distinct_primes((uint64_t)n); /* ω(n) */
    seq_t diff = (omega > 0) ? omega - 1 : 0;  /* avoid unsigned underflow for n=1 */
    return diff * distinct + 2;
}
