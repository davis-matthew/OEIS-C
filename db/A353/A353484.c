/*
 * A353484  a(1) = 0; and for n > 1, a(n) = A165560(n) + a(A064989(n)), where
 *          A165560 is the parity of arithmetic derivative, and A064989 shifts
 *          the prime factorization of its argument one step toward lower
 *          primes.
 * https://oeis.org/A353484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = parity_derivative(n) + a(prime_shift(n)), with a(1) = 0.
 *            This implementation uses an iterative computation without
 *            recursion or memoisation. For each n we repeatedly apply the
 *            prime‑shift operation until reaching 1, summing the parity of
 *            the arithmetic derivative at each step
 * Range:     u8 n <= 1618, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9649 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Parity of the arithmetic derivative of n.
   The arithmetic derivative d(n) = Σ e·n/p over all prime powers p^e‖n.
   The parity is the XOR of the parities of the individual terms. */
static int _parity_derivative(idx_t n)
{
    if (n <= 1U) return 0;

    int parity = 0;
    idx_t t = n;

    /* Handle factor 2 */
    int e2 = 0;
    while ((t & 1U) == 0U) { t >>= 1; ++e2; }
    /* Term e2 * (n/2) is odd iff e2 is odd and (n/2) is odd. */
    if ((e2 & 1) && ((n >> 1) & 1U))
        parity ^= 1;

    /* Odd prime factors */
    for (idx_t p = 3U; p * p <= t; p += 2U)
    {
        if (t % p == 0U)
        {
            int e = 0;
            while (t % p == 0U) { t /= p; ++e; }
            /* Only odd exponents contribute */
            if (e & 1)
                parity ^= (int)((n / p) & 1U);
        }
    }

    if (t > 1U) /* remaining prime factor */
    {
        /* exponent is 1 (odd) */
        parity ^= (int)((n / t) & 1U);
    }

    return parity;
}

/* ------------------------------------------------------------------ */
/* Small integer utilities                                             */
/* ------------------------------------------------------------------ */

/* Simple deterministic primality test for 64‑bit integers
   (adequate for the tiny numbers encountered in this sequence) */
static bool _is_prime_u64(idx_t x)
{
    if (x < 2U) return false;
    if (x == 2U) return true;
    if ((x & 1U) == 0U) return false;
    for (idx_t d = 3U; d * d <= x; d += 2U)
        if (x % d == 0U) return false;
    return true;
}

/* Return the largest prime less than p.
   p is guaranteed to be an odd prime > 2. */
static idx_t _prev_prime(idx_t p)
{
    /* For the very small inputs used here a linear scan is fine */
    for (idx_t q = p - 1U; q >= 2U; --q)
        if (_is_prime_u64(q))
            return q;
    return 1U; /* unreachable for valid odd primes > 2 */
}

/* ------------------------------------------------------------------ */
/* Core helpers                                                        */
/* ------------------------------------------------------------------ */

/* Prime‑shift of n: drop all factors of 2, replace every odd prime
   factor p^e by prev_prime(p)^e.  If n ≤ 1, return 1 (the
   representation of 1 after shifting). */
static idx_t _prime_shift(idx_t n)
{
    if (n <= 1U) return 1U;

    idx_t res = 1U;

    /* Remove all 2‑factors */
    while ((n & 1U) == 0U) n >>= 1;

    /* Process odd prime factors */
    for (idx_t p = 3U; p * p <= n; p += 2U)
    {
        if (n % p == 0U)
        {
            int e = 0;
            while (n % p == 0U) { n /= p; ++e; }
            idx_t prev = _prev_prime(p);
            for (int i = 0; i < e; ++i)
                res *= prev;
        }
    }

    if (n > 1U) /* remaining prime factor */
    {
        idx_t prev = _prev_prime(n);
        res *= prev;
    }

    return res;
}

/* ------------------------------------------------------------------ */
/* Public API required by the build system                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 1U)
        return 0U;

    seq_t result = 0U;
    idx_t cur = n;

    while (cur > 1U)
    {
        result += (seq_t)_parity_derivative(cur);
        cur = _prime_shift(cur);
    }

    return result;
}
