/*
 * A316553  Number of elements of order 2 in the group SL(2, Z(n)).
 * https://oeis.org/A316553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(omega(n) + min(3, valuation(n, 2))) - 1
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 95 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* for completeness – n>=1 in use */

    /* valuation of 2 in n */
    idx_t v2 = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* number of distinct prime factors */
    int omega_int = count_distinct_primes((uint64_t)n);
    idx_t omega = (idx_t)omega_int;

    /* exponent for 2^exp */
    idx_t exp = omega + (v2 < 3 ? v2 : 3ULL);

    /* 2^exp minus 1 */
    seq_t val = pow2(exp);
    val -= (seq_t)1;
    return val;
}
