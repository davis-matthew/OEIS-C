/*
 * A354870  Number of nonprime squarefree divisors of n.
 * https://oeis.org/A354870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ω(n) - ω(n) where ω(n) is the number of distinct prime
 *            factors of n. A(n) counts the square‑free divisors of n that are
 *            not primes, i.e. 1 and any composite square‑free divisor
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9484 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined only for n >= 1 */

    /* distinct prime factor count ω(n) */
    int omega = count_distinct_primes((uint64_t)n);

    /* compute 2^ω(n) using only seq_t arithmetic */
    seq_t sqfree = (seq_t)1;
    for (int i = 0; i < omega; ++i)
        sqfree = sqfree * (seq_t)2;

    /* subtract the ω(n) primes themselves */
    return sqfree - (seq_t)omega;
}
