/*
 * A323172  Denominator of the average of distinct prime factors of n
 *          (A008472(n)/A001221(n)).
 * https://oeis.org/A323172
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = A001221(n)/gcd(A008472(n),A001221(n)) where A008472(n) is
 *            the sum of the distinct prime factors of n and A001221(n) is the
 *            number of distinct prime factors of n. (Denominator of the
 *            reduced fraction.)
 * Range:     u8 n <= 9614, u16 n <= 9614, u32 n <= 9614, u64 n <= 9614,
 *            u128 n <= 9614, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for gcd on uint64_t (safe for all intermediate values). */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Compute a(n) for n ≥ 2. */
seq_t a(idx_t n)
{
    if (n < 2)          /* values below the offset are irrelevant */
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    uint64_t sum   = 0;   /* sum of distinct primes */
    uint64_t count = 0;   /* number of distinct primes */

    /* Handle factor 2 separately for speed. */
    if ((m & 1) == 0) {
        sum += 2;
        count++;
        while ((m & 1) == 0)
            m >>= 1;
    }

    /* Trial division by odd primes up to sqrt(m). */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            sum += p;
            count++;
            while (m % p == 0)
                m /= p;
        }
    }

    /* If any prime factor remains. */
    if (m > 1) {
        sum += m;
        count++;
    }

    /* Reduce the fraction sum/count and return the denominator. */
    uint64_t g = gcd_uint64(sum, count);
    uint64_t denom = count / g;

    return (seq_t)denom;   /* denom is ≤ count, thus ≤ 8 for n ≤ 10000 */
}
