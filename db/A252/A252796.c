/*
 * A252796  Smallest of n consecutive integers whose product is divisible by
 *          at least n distinct prime factors.
 * https://oeis.org/A252796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search using a smallest‑prime‑factor sieve
   to find the smallest m such that the product m⋯(m+n−1) has at least n
   distinct prime divisors.  The function memoises results in a static
   array so that calls with arbitrary n are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Limit of the sieve – big enough for n up to 10 000 (p_10000≈104 729). */
static const idx_t LIMIT = 200000;

/* Prime‑factorisation helpers.  All allocated/initialised only once. */
static uint32_t *spf        = NULL;   /* smallest prime factor of i */
static int      *prime_id   = NULL;   /* prime i → 0‑based index, −1 for composites */
static uint32_t *last_seen  = NULL;   /* last window marker for each prime */
static size_t     prime_cnt = 0;

/* Memoisation of the sequence: cache[1] = a(1) etc. */
static seq_t *cache      = NULL;   /* 1‑based, cache[0] unused */
static size_t cache_size = 0;      /* current allocated size (>= cache[n] can be read) */
static idx_t  last_start = 2;       /* starting value used for the previous n */
static bool   initialized = false;

/* ------------------------------------------------------------------ */
/* Initialise the sieve and helper tables.                            */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;

    /* Smallest‑prime‑factor array */
    spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* Map primes to consecutive indices and count them. */
    prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == (uint32_t)i) ++prime_cnt;

    prime_id = (int*)malloc((LIMIT + 1) * sizeof(int));
    memset(prime_id, -1, (LIMIT + 1) * sizeof(int));
    int pid = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == (uint32_t)i) prime_id[i] = pid++;

    last_seen = (uint32_t*)calloc(prime_cnt, sizeof(uint32_t));

    /* Allocate memoisation array (index 0 unused). */
    cache = (seq_t*)malloc(sizeof(seq_t));
    cache[0] = (seq_t)0;
    cache_size = 1;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Test whether the product of `len` consecutive numbers starting at
   `start` contains at least `len` distinct prime divisors.            */
/* ------------------------------------------------------------------ */
static bool window_has_n_primes(idx_t start, idx_t len)
{
    static uint32_t current_mark = 0;
    ++current_mark;
    if (current_mark == 0) {            /* overflow – reset table */
        memset(last_seen, 0, prime_cnt * sizeof(uint32_t));
        current_mark = 1;
    }

    uint32_t distinct = 0;
    for (idx_t i = 0; i < len; ++i) {
        idx_t x = start + i;
        while (x > 1) {
            uint32_t p = spf[x];
            int pid = prime_id[p];
            if (last_seen[pid] != current_mark) {
                last_seen[pid] = current_mark;
                ++distinct;
                if (distinct >= len) return true;
            }
            while (x % p == 0) x /= p;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Public sequence function `a(n)` – external linkage.               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    init();                      /* ensure tables exist */

    /* Compute values up to n if not already cached. */
    for (idx_t k = cache_size; k <= n; ++k) {
        idx_t start = (k == 1) ? 2 : last_start;
        while (!window_has_n_primes(start, k)) ++start;

        /* Resize cache to hold a(k). */
        cache = (seq_t*)realloc(cache, (k + 1) * sizeof(seq_t));
        cache[k] = (seq_t)start;
        cache_size = k + 1;
        last_start = start;
    }

    return cache[n];
}
