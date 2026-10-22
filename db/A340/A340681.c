/*
 * A340681  The closure under squaring of A051144, the nonsquarefree
 *          nonsquares.
 * https://oeis.org/A340681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340681: numbers not of the form s^(2^e) where s is square‑free.
   Computation: test each integer for that property and keep the non‑special
   ones in a static cache.  The cache is large enough for all required
   queries with n ≤ 10000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_CACHE 50000   /* sufficient for the first 10000 sequence terms */

static seq_t cache[MAX_CACHE];
static size_t cache_count = 0;
static idx_t next_int = 2;   /* integers tested so far */

/* Test whether x is of the form s^(2^e) with s square‑free. */
static bool is_special(uint64_t x)
{
    if (x <= 1) return true;          /* 1 = 1^1, 1 is square‑free */

    uint64_t t = x;
    int first_exp = -1;
    int cnt = 0;

    /* factor 2 */
    while ((t & 1U) == 0) { t >>= 1; ++cnt; }
    if (cnt > 0) first_exp = cnt;

    /* factor odd primes */
    for (uint64_t p = 3; p <= t / p; p += 2)
    {
        if (t % p == 0)
        {
            cnt = 0;
            while (t % p == 0) { t /= p; ++cnt; }
            if (first_exp == -1) first_exp = cnt;
            else if (cnt != first_exp) return false;
        }
    }

    /* remaining prime factor > sqrt(t) */
    if (t > 1)
    {
        cnt = 1;
        if (first_exp == -1) first_exp = cnt;
        else if (cnt != first_exp) return false;
    }

    /* all exponents equal and a power of two? */
    return ((first_exp & (first_exp - 1)) == 0);
}

/* Return the nth term of A340681 (a(1) = 8). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    while (cache_count < (size_t)n)
    {
        if (!is_special((uint64_t)next_int))
        {
            cache[cache_count++] = (seq_t)next_int;
            if (cache_count >= MAX_CACHE) /* safety check */
                break;
        }
        ++next_int;
    }
    return cache[n - 1];
}
