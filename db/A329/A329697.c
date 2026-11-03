/*
 * A329697  a(n) is the number of iterations needed to reach a power of 2
 *          starting at n and using the map k -> k-(k/p), where p is the
 *          largest prime factor of k.
 * https://oeis.org/A329697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n is a power of 2; otherwise a(n) = 1 + a(n -
 *            n/LPF(n)), where LPF(n) is the largest prime factor of n.
 *            Recurrence is memoised only by recursion; works for all n ≥ 1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9519 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

static seq_t a_rec(idx_t n)
{
    /* base cases */
    if (n <= 2)
        return 0;

    /* if n is a power of two → 0 */
    if ((n & (n - 1)) == 0)
        return 0;

    /* strip off all factors of 2 – they do not affect the result */
    while ((n & 1) == 0)
        n >>= 1;

    /* largest prime factor of the remaining odd part */
    uint64_t lpf = largest_prime_divisor((uint64_t)n);
    if (lpf == 0)          /* safety guard – never happens for n > 1 */
        lpf = n;

    /* next value in the trajectory */
    idx_t next = n - n / lpf;

    /* recursion – plus one iteration */
    return 1 + a_rec(next);
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
