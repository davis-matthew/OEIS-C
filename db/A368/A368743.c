/*
 * A368743  a(n) = Sum_{1 <= i, j <= n} gcd(i, j, n)^3.
 * https://oeis.org/A368743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Σ_{d|n} d^3 * J₂(n/d) (multiplicative) a(p^k) =
 *            p^(3k‑2)(p²+p+1) – p^(2k‑2)(p+1) (k≥1)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1199, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are defined by the build.  The function works for
    any n ≥ 1, and is fast (O(√n)).  No external utilities are
    required.  */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- helper: fast power in seq_t ---------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- helper: value of a for a prime power ---------- */
static seq_t a_prime_power(uint64_t p, idx_t k)
{
    seq_t pp = (seq_t)p;
    /* p^(3k‑2) */
    seq_t p3 = pow_seq(pp, (idx_t)(3 * k - 2));
    /* p^(2k‑2) */
    seq_t p2 = pow_seq(pp, (idx_t)(2 * k - 2));
    /* (p² + p + 1) and (p + 1) */
    seq_t t1 = pp * pp + pp + 1;
    seq_t t2 = pp + 1;
    return p3 * t1 - p2 * t2;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* outside the OEIS offset – harmless */

    /* a(1) = 1 */
    if (n == 1)
        return 1;

    seq_t result = 1;
    idx_t m = n;

    /* factor 2 separately */
    if ((m & 1) == 0)
    {
        idx_t k = 0;
        while ((m & 1) == 0)
        {
            m >>= 1;
            ++k;
        }
        result *= a_prime_power(2, k);
    }

    /* factor odd primes */
    for (idx_t d = 3; d * d <= m; d += 2)
    {
        if (m % d == 0)
        {
            idx_t k = 0;
            while (m % d == 0)
            {
                m /= d;
                ++k;
            }
            result *= a_prime_power((uint64_t)d, k);
        }
    }

    /* if something remains it's a prime > sqrt(n) */
    if (m > 1)
        result *= a_prime_power((uint64_t)m, 1);

    return result;
}
