/*
 * A249767  a(n) = mu(n) + omega(n).
 * https://oeis.org/A249767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mu(n) + omega(n)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 103 terms through n = 4911 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* omega(n) = number of distinct prime factors of n */
/* mu(n) is the Möbius function: */
/*   μ(1) = 1; μ(n) = 0 if n is not square‑free; μ(n) = (–1)^k when n is a product of k distinct primes */

#include <stdint.h>
#include <stdbool.h>

static seq_t mobius(idx_t m)
{
    if (m == 0) return 0;          /* defined as 0, but 0 is never passed */
    seq_t sign = 1;                /* product of (–1) for each prime factor */
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
                if (cnt > 1)
                    return 0;     /* not square‑free */
            }
            sign = -sign;
        }
    }
    if (tmp > 1)                   /* remaining prime factor */
        sign = -sign;
    return sign;
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
    if (n == 0)          /* below offset 1 – undefined for the sequence */
        return 0;

    /* Möbius value as a signed 8‑bit integer */
    int8_t mu = (int8_t)mobius(n);             /* mobius returns seq_t */

    /* ω(n): number of distinct prime factors */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    seq_t result;
    if (mu == -1)            /* primes: μ = –1, ω = 1 → result = 0 */
        result = omega - 1;
    else                     /* μ = 0 or 1 */
        result = omega + (seq_t)mu;

    return result;
}
