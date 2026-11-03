/*
 * A347245  Number of iterations of the map x -> A000593(x), when starting
 *          from x = n, needed to reach a number whose largest prime factor is
 *          at least as large as that of n itself (= A006530(n)). If 1 is
 *          reached without encountering such a number, then a(n) = 0.
 * https://oeis.org/A347245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of the map x -> sum of odd divisors
 *            of x, starting from x = n, until a number whose largest prime
 *            factor is >= that of n is reached. If 1 is reached first, a(n) =
 *            0
 * Range:     u8 n <= 2540, u16 n <= 102752, u32 n <= 102753, u64 n <= 102753,
 *            u128 n <= 102753, big any n
 * Validated: standard, pass, 137 terms through n = 9179 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helpers from utils. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

seq_t a(idx_t n)
{
    /* Special case: n = 1 has no prime factors; by definition a(1)=0. */
    if (n <= 1)
        return 0;

    /* Largest prime factor of the starting value. */
    uint64_t start_lpf = largest_prime_divisor((uint64_t)n);

    uint64_t cur = n;      /* current value on each step (kept 64‑bit). */
    seq_t  cnt  = 0;       /* number of steps taken. */

    while (1) {
        /* Compute sum of odd divisors of cur.
           Remove all factors of 2 first. */
        uint64_t odd = cur;
        while ((odd & 1ULL) == 0ULL)
            odd >>= 1;

        /* sigma_of returns a seq_t, which we expect to fit in 64‑bit
           for the sizes used in the tests.  Convert it back to uint64_t. */
        cur = (uint64_t)sigma_of(odd);
        ++cnt;

        /* If 1 is reached, no suitable number was found. */
        if (cur == 1)
            return 0;

        /* Check the largest prime factor of the new value. */
        uint64_t cur_lpf = largest_prime_divisor(cur);
        if (cur_lpf >= start_lpf)
            return cnt;
    }
}
