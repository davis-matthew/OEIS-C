/*
 * A333175  If n = Product (p_j^k_j) then a(n) = Sum (a(n/p_j^k_j)), with a(1)
 *          = 1.
 * https://oeis.org/A333175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = factorial(omega(n)), where omega(n) is the number of
 *            distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9936 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Computes factorial for small integers k (k <= 20) and returns it as seq_t. */
static seq_t factorial_uint32(uint32_t k) {
    seq_t res = 1;
    for (uint32_t i = 2; i <= k; ++i)
        res = res * (seq_t)i;
    return res;
}

seq_t a(idx_t n) {
    /* n is at least 1 (offset 1). */
    /* Count distinct prime factors of n. */
    int omega = count_distinct_primes((uint64_t)n);

    /* Return factorial of omega. */
    /* For omega = 0 (n=1), factorial(0) = 1. */
    return factorial_uint32((uint32_t)omega);
}
