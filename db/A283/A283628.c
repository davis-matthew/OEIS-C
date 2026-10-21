/*
 * A283628  Numbers m such that 4m - 3, 4m - 2, 4m - 1, 4m + 1, 4m + 2 and 4m
 *          + 3 are all squarefree.
 * https://oeis.org/A283628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer m such that 4m‑3, 4m‑2, 4m‑1, 4m+1, 4m+2, 4m+3 are all square‑free.
   Computed by successive search with an efficient square‑free test. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint32_t primes[1000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* Build a small list of primes up to 1000 – enough for 4m+3 with m up to ~60 000. */
static void init_primes(void)
{
    if (primes_ready)
        return;
    const uint32_t limit = 1000;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    sieve[0] = sieve[1] = true;          /* mark 0,1 as non‑prime */
    for (uint32_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                sieve[j] = true;

    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            primes[prime_cnt++] = i;

    free(sieve);
    primes_ready = true;
}

/* True iff n has no squared prime divisor.  n is small enough to fit in uint64_t. */
static bool is_sqfree(uint64_t n)
{
    if (n < 2) /* 0,1 are square‑free by definition */
        return true;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > n)
            break;
        if (n % p == 0)
        {
            n /= p;
            if (n % p == 0)
                return false;            /* squared factor found */
        }
    }
    return true;                         /* no squared factors */
}

static uint64_t *cache   = NULL;   /* found values of m, as uint64_t */
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static uint64_t current_m = 0;       /* last m tested */

/* ensure the dynamic array has room for at least len elements */
static void ensure_cache_capacity(size_t len)
{
    if (cache_cap >= len)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < len)
        new_cap = len;
    uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();              /* out of memory – terminate program */
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();                     /* make primes available */

    if (n == 0)                         /* problem guarantees n >= 1 */
        return (seq_t)0;

    /* grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        ++current_m;                    /* next candidate m */

        bool ok = true;
        uint64_t base = 4 * current_m - 3;
        for (int k = 0; k <= 6 && ok; ++k)
        {
            uint64_t val = base + (uint64_t)k;   /* 4m-3 + k */
            if (val == 4 * current_m)           /* skip 4m, not square‑free */
                continue;
            if (!is_sqfree(val))
                ok = false;
        }

        if (ok)
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = current_m;
        }
    }

    /* return the requested term, converting to seq_t */
    return (seq_t)cache[n - 1];
}
