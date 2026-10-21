/*
 * A268426  Primes of the form p^2 + 12*q^2, p, q primes.
 * https://oeis.org/A268426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes of the form p^2 + 12*q^2, p, q primes
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper: trial‑division primality test for seq_t values.            */
/* ------------------------------------------------------------------ */
static bool is_prime(seq_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if ((n & 1) == 0)
        return false;

    for (seq_t i = 3; i <= n / i; i += 2)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Helper: integer square root (floor) for seq_t values.              */
/* ------------------------------------------------------------------ */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t sq = mid * mid;
        if (sq == x)
            return mid;
        else if (sq < x)
            lo = mid + 1;
        else
            hi = (mid == 0) ? 0 : mid - 1;  /* avoid underflow */
    }
    return hi;  /* floor sqrt */
}

/* ------------------------------------------------------------------ */
/* Helper: test whether a number n can be written as p^2 + 12*q^2      */
/* where p and q are prime.                                            */
/* ------------------------------------------------------------------ */
static bool satisfies_form(seq_t n)
{
    for (seq_t q = 2; /* q will be limited internally */ ; ++q)
    {
        /* 12*q^2 must not exceed n. Use division to avoid overflow.  */
        seq_t qsq = q * q;
        if (qsq > n / 12)
            break;
        if (!is_prime(q))
            continue;

        seq_t r = n - 12 * qsq;
        if (r == 0)
            continue;
        seq_t sqrt_r = floor_sqrt(r);
        if (sqrt_r * sqrt_r == r && is_prime(sqrt_r))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Cached sequence storage.                                           */
/* ------------------------------------------------------------------ */
static seq_t *cached = NULL;     /* array of found primes */
static size_t cached_count = 0;  /* number of elements stored */
static size_t cached_cap   = 0;  /* capacity of the array */
static seq_t next_candidate = 2; /* next integer to test */

/* ------------------------------------------------------------------ */
/* Main function required by the build system.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Extend cache until we have at least n values.                 */
    while (cached_count < (size_t)n)
    {
        seq_t m = next_candidate;

        /* Skip to the next prime.                                   */
        while (!is_prime(m))
            ++m;

        /* Check the form p^2 + 12*q^2.                               */
        if (satisfies_form(m))
        {
            /* Grow the cache if needed. */
            if (cached_count == cached_cap)
            {
                size_t new_cap = cached_cap ? cached_cap * 2 : 16;
                seq_t *new_arr = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
                if (!new_arr) /* allocation failed */
                    abort();
                cached = new_arr;
                cached_cap = new_cap;
            }
            cached[cached_count++] = m;
        }

        /* Advance the candidate for the next iteration.              */
        next_candidate = m + 1;
    }

    return cached[n - 1];
}
