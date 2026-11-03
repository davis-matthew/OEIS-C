/*
 * A327829  Squarefree numbers with a prime number of prime factors.
 * https://oeis.org/A327829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8615 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A327829 : squarefree integers whose number of distinct prime factors is a prime.
 * The function returns the n-th term (n ≥ 1) of this sequence.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether x is square‑free.  Simple trial division works
   because numbers in the sequence are relatively small for the
   required range of n. */
static bool is_squarefree(seq_t x)
{
    for (idx_t d = 2; (seq_t)d * (seq_t)d <= x; ++d) {
        if (x % d == 0) {
            x /= d;
            if (x % d == 0) return false;
        }
    }
    return true;
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

/* Test whether a small seq_t-valued integer is prime.
   Works for all supported seq_t types. */
static bool is_prime_small(seq_t num) {
    if (num < 2) return false;
    for (seq_t d = 2; d * d <= num; ++d) {
        if (num % d == 0) return false;
    }
    return true;
}

/* a(n) : the n-th squarefree integer with a prime number of distinct prime factors */
seq_t a(idx_t n)
{
    /* ------------- static memoisation state -------------------- */
    static seq_t *cache      = NULL;   /* cached terms */
    static size_t cache_cap = 0;         /* capacity of cache array  */
    static size_t cache_len = 0;         /* number of terms cached  */
    static uint64_t last_checked = 0;    /* last integer examined   */

    /* ------------- build cache until we have n terms ----------- */
    while (cache_len < (size_t)n)
    {
        ++last_checked;
        uint64_t cand = last_checked;

        /* fast square‑free test */
        if (is_squarefree((seq_t)cand))
        {
            /* count distinct prime factors */
            int omega = count_distinct_primes(cand);

            /* check that this count itself is prime (small < 64) */
            if (is_prime_small((seq_t)omega))
            {
                /* grow cache if necessary */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap ? cache_cap * 2 : 8);
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();                /* memory exhausted */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
            }
        }
    }

    /* ------------- return cached term -------------------------- */
    return cache[n - 1];
}
