/*
 * A324847  Numbers divisible by at least one of their prime indices.
 * https://oeis.org/A324847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer such that there exists * m
 *            where prime(m) divides the integer and m also divides it
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime sieve utilities                                           */
/* ------------------------------------------------------------------ */
#define MAX_PRIME 400000         /* enough for n up to about 10 000 */

static bool primes_initialized = false;
static uint32_t primes[MAX_PRIME];
static uint32_t prime_index[MAX_PRIME + 1];   /* 1‑based index of prime, 0 if not prime */
static size_t prime_count = 0;

/* Initialize the prime tables */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    bool *is_composite = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    if (!is_composite)
        abort();

    for (size_t i = 2; i <= (size_t)MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[prime_count] = (uint32_t)i;
            prime_index[i] = (uint32_t)(prime_count + 1);  /* 1‑based index */
            ++prime_count;
            if (i <= MAX_PRIME / i) {          /* avoid overflow */
                for (size_t j = i * i; j <= (size_t)MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Test whether a number satisfies the definition                    */
/* ------------------------------------------------------------------ */
static bool satisfies(idx_t n)
{
    uint64_t temp = (uint64_t)n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > temp)
            break;
        if (temp % p == 0) {
            uint32_t k = prime_index[p];
            if (k > 0 && (n % k) == 0)
                return true;
            do
                temp /= p;
            while (temp % p == 0);
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        uint32_t k = prime_index[(uint32_t)temp];
        if (k > 0 && (n % k) == 0)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Cache for the sequence elements                                  */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t next_num = 2;          /* next integer to test */

/* Ensure the cache contains at least 'n' elements */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    while (cache_size < (size_t)n) {
        while (!satisfies(next_num))
            ++next_num;
        /* store value */
        cache[cache_size++] = (seq_t)next_num;
        ++next_num;
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            seq_t *p = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!p)
                abort();
            cache = p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();      /* ensure primes are ready */
    ensure_cache(n);    /* build cache up to requested n */
    return cache[n - 1]; /* indices are 0‑based in the cache */
}
