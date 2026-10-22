/*
 * A337903  a(0) = 0, a(1) = 1; for n>1, if a(n-1) is composite then a(n) =
 *          a(n-1)/A107286(n-1), where A107286(n) = the smallest prime factor
 *          of n, otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A337903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; for n>1: if a(n-1) is composite then a(n) =
 *            a(n-1)/A107286(n-1), otherwise a(n) = a(n-1) + n (where
 *            A107286(m) is the smallest prime factor of m)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple trial‑division primality test.  Works for 64‑bit and
   arbitrary-precision seq_t values. */
static bool is_prime(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (seq_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Smallest prime factor of a composite seq_t.  Called only when n>1
   and not prime (i.e. truly composite). */
static seq_t spf_of(seq_t n)
{
    if (n % 2 == 0) return 2;
    for (seq_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return d;
    /* Should not reach here for a composite input, but return n
       as a fallback to avoid undefined behaviour. */
    return n;
}

/* Cache for previously computed terms.  cache[0] = a(0), cache[1] = a(1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache holds values up to index n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    /* Initialise cache for the first two terms if not already done. */
    if (cache_len == 0)
    {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* Grow and compute new terms if necessary. */
    if (n + 1 > cache_len)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t prev = cache[i - 1];
            /* Treat 1 as non‑composite (additive case). */
            if (prev == 1 || is_prime(prev))
                cache[i] = prev + (seq_t)i;
            else
                cache[i] = prev / spf_of(prev);
        }
        cache_len = n + 1;
    }
}

/* OEIS sequence a(n).  Works for arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    ensure_cache_up_to(n);
    return cache[n];
}
