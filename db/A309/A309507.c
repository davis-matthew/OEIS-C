/*
 * A309507  Number of ways the n-th triangular number T(n) = A000217(n) can be
 *          written as the difference of two positive triangular numbers.
 * https://oeis.org/A309507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d_odd(n) * d_odd(n+1) - 1 * where d_odd(k) is the number
 *            of odd divisors of k (including 1). * Since n and n+1 are
 *            coprime, d_odd(n*(n+1)) = d_odd(n)*d_odd(n+1). * The desired
 *            count excludes the divisor 1, hence the final "- 1"
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 8827 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdint.h>  /* for uint64_t, if needed */

/* Count the odd divisors of k (including 1). */
static seq_t count_odd_divisors(idx_t k)
{
    /* Remove all factors of 2. */
    while ((k & 1) == 0)
        k >>= 1;

    seq_t count = 1;                 /* divisor 1 is always counted */
    idx_t m = k;

    /* Trial division by odd numbers only. */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            count *= (e + 1);        /* multiply by (exponent + 1) */
        }
    }
    if (m > 1)                       /* remaining prime factor */
        count *= 2;                  /* exponent 1 -> (1+1) */

    return count;
}

/* OEIS A309507: number of ways T(n) can be expressed as difference of two positive triangular numbers. */
seq_t a(idx_t n)
{
    if (n == 0)        /* not required for offset 1, but safe guard */
        return 0;

    seq_t d1 = count_odd_divisors(n);
    seq_t d2 = count_odd_divisors(n + 1);

    return d1 * d2 - 1;
}
