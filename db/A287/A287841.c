/*
 * A287841  Number of iterations of number of distinct prime factors (A001221)
 *          needed to reach 1 starting at n (n is counted).
 * https://oeis.org/A287841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(omega(n)) + 1 for n > 1; a(1) = 1
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9003 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  omega(n) is the number of distinct prime factors of n           */
#include <stdint.h>
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

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    /* compute the number of distinct prime factors of n */
    int omega = count_distinct_primes((uint64_t)n);
    /* recursive definition: a(n) = a(omega(n)) + 1 */
    return a((idx_t)omega) + (seq_t)1;
}
