/*
 * A356172  a(n) = 1 if n is odd and not divisible by two consecutive prime
 *          numbers, otherwise 0.
 * https://oeis.org/A356172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and not divisible by two consecutive prime
 *            numbers, otherwise 0. This implementation factors n, collects
 *            its distinct prime divisors, and checks for each whether the
 *            next prime also divides n. For even n the result is immediately
 *            0. For n = 1 the result is 1
 * Range:     u8 n <= 9758, u16 n <= 9758, u32 n <= 9758, u64 n <= 9758,
 *            u128 n <= 9758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Forward declarations of helper functions local to this file. */
static bool _is_prime(idx_t n);
static idx_t _next_prime(idx_t n);

/*--------------------------------------------------------------------
 * a(n) – OEIS A356172
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 1. Even numbers are always 0. */
    if ((n & 1ULL) == 0)
        return (seq_t)0;

    /* 2. n == 1 is a special case that satisfies the condition. */
    if (n == 1)
        return (seq_t)1;

    /* 3. Factor n and store distinct prime factors. */
    idx_t temp = n;
    idx_t primes[32];
    idx_t pf_len = 0;

    for (idx_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            /* Store distinct prime factor. */
            if (pf_len < 32)
                primes[pf_len++] = d;
            /* Divide out all copies of d. */
            while (temp % d == 0)
                temp /= d;
        }
    }
    /* If remainder is a prime > sqrt(original n), store it. */
    if (temp > 1) {
        if (pf_len < 32)
            primes[pf_len++] = temp;
    }

    /* 4. For each distinct prime factor p, check if the next prime q
       also divides n.  If such a pair exists, return 0. */
    for (idx_t i = 0; i < pf_len; ++i) {
        idx_t p = primes[i];
        idx_t q = _next_prime(p);   /* smallest prime strictly greater than p */
        if (n % q == 0)
            return (seq_t)0;
    }

    /* 5. No consecutive prime pair divides n; return 1. */
    return (seq_t)1;
}

/*--------------------------------------------------------------------
 * Helper: returns true iff n is a prime (trial division).
 *--------------------------------------------------------------------*/
static bool _is_prime(idx_t n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if ((n & 1ULL) == 0)
        return false;
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

/*--------------------------------------------------------------------
 * Helper: returns the smallest prime strictly greater than n.
 *--------------------------------------------------------------------*/
static idx_t _next_prime(idx_t n)
{
    idx_t candidate = n + 1;
    for (;;) {
        if (_is_prime(candidate))
            return candidate;
        ++candidate;
    }
}
