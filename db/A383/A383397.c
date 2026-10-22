/*
 * A383397  Numbers in whose canonical prime factorization the powers of the
 *          primes form a strictly increasing sequence.
 * https://oeis.org/A383397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   generates the n-th term of A383397 – the positive integer whose
 *            * canonical prime powers form a strictly increasing sequence. *
 *            The function enumerates successive integers, tests the property,
 *            * and memoises the terms so that later calls are O(1)
 * Range:     u8 n <= 223, u16 n <= 9908, u32 n <= 9908, u64 n <= 9908,
 *            u128 n <= 9908, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime sieve (once) ---------- */
static uint32_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static bool     primes_ok   = false;

static void init_primes(void)
{
    if (primes_ok) return;
    const uint32_t limit = 200000;                /* enough for the needed n */

    bool *sieve = (bool *)calloc((size_t)limit + 1U, (size_t)1U); /* 0 = prime */
    for (uint32_t p = 2; p * p <= limit; ++p)
        if (!sieve[p])
            for (uint32_t m = p * p; m <= limit; m += p)
                sieve[m] = true;

    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[idx++] = i;
    prime_cnt = cnt;
    free(sieve);
    primes_ok = true;
}

/* ---------- check whether a number satisfies the property ---------- */
static bool valid(seq_t value)
{
    seq_t prev = 0;
    seq_t x = value;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        seq_t pseq = (seq_t)p;
        if (pseq * pseq > x) break;
        if (x % p != 0) continue;
        seq_t cur = 1;
        do { cur *= pseq; x /= p; } while (x % p == 0);
        if (prev != 0 && prev >= cur) return false;
        prev = cur;
    }
    if (x > 1) {            /* remaining prime factor */
        seq_t cur = x;
        if (prev != 0 && prev >= cur) return false;
    }
    return true;
}

/* ---------- memoisation of computed terms ---------- */
static seq_t *cache        = NULL;
static size_t cache_cap    = 0;
static size_t cache_len    = 0;
static seq_t  last_return  = 0;   /* last number examined */

static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 512 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* main function as required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but guard */
    init_primes();

    /* return cached value if already known */
    if ((size_t)(n - 1) < cache_len) return cache[n - 1];

    /* compute successive terms until we reach n */
    for (seq_t cand = (seq_t)last_return + 1; ; ++cand) {
        if (valid(cand)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = cand;
            last_return = cand;
            if ((size_t)(n - 1) < cache_len) return cand;
        }
    }
}
