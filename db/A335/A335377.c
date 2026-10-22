/*
 * A335377  Heinz numbers of non-totally co-strong integer partitions.
 * https://oeis.org/A335377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_totally_co_strong_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth Heinz number of a partition that is not
   totally co‑strong.  The function builds the sequence incrementally:
   it scans integers, tests whether their prime‑index partition is
   non‑totally co‑strong, and caches the accepted values.  Calls can
   arrive in any order.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_totally_co_strong_uint32.h"

/* ---------- prime sieve (needed for factorisation) ---------- */
#define PRIME_SIEVE_MAX 2000000   /* covers all primes needed for n up to ~1e5 */

/* static cache of primes (1‑based order: prime(1)=2) */
static uint32_t *prime_list   = NULL;
static size_t   prime_count   = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;                 /* already initialised */

    size_t limit = PRIME_SIEVE_MAX;

    /* simple sieve of Eratosthenes */
    unsigned char *sieve = (unsigned char *)malloc(limit + 1);
    memset(sieve, 1, limit + 1);
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    /* allocate list */
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    free(sieve);
}

/* ---------- factorisation: produce prime indices (1‑based) and exponents ---------- */
static void factor_uint64(uint64_t n,
                          uint32_t *idx_arr,  /* prime indices (1‑based) */
                          uint8_t *exps,      /* exponents          */
                          size_t *len)
{
    size_t idx = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            uint8_t c = 0;
            while (n % p == 0) {
                n /= p;
                ++c;
            }
            idx_arr[idx] = (uint32_t)(i + 1); /* 1‑based index */
            exps[idx]    = c;
            ++idx;
        }
    }
    if (n > 1) {
        /* n is a prime; find its index via binary search */
        size_t lo = 0, hi = prime_count;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] < n) lo = mid + 1;
            else hi = mid;
        }
        /* lo now is the index of n in the list */
        idx_arr[idx] = (uint32_t)(lo + 1);   /* 1‑based */
        exps[idx]    = 1;
        ++idx;
    }
    *len = idx;
}

/* ---------- test whether a number is a Heinz number of a non‑totally co‑strong partition ---------- */
static bool is_non_totally_co_strong(uint64_t n)
{
    if (n <= 1) return false;   /* 1 is not in the sequence */

    /* factorise n and obtain prime indices and exponents */
    uint32_t idx_arr[64];
    uint8_t   exps[64];
    size_t    len = 0;
    factor_uint64(n, idx_arr, exps, &len);

    /* build the partition: each prime index repeated according to its exponent */
    size_t parts_len = 0;
    uint32_t parts[256];
    for (size_t i = len; i-- > 0; ) {
        for (uint8_t k = 0; k < exps[i]; ++k)
            parts[parts_len++] = idx_arr[i];
    }

    /* return true if partition is non‑totally co‑strong */
    return !is_totally_co_strong_uint32(parts, parts_len);
}

/* ---------- cache for memoised sequence values ---------- */
static seq_t *cache          = NULL;   /* array of computed values */
static size_t cache_len       = 0;     /* number of cached values      */
static size_t cache_cap       = 0;     /* allocated capacity           */
static idx_t  last_checked    = 1;     /* last integer tested          */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, safe default */

    init_primes();                   /* ensure prime list is ready */

    /* generate values until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++last_checked;              /* next integer to test */
        if (is_non_totally_co_strong((uint64_t)last_checked)) {
            if (cache_len == cache_cap) {        /* grow cache if needed */
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)(n - 1)];
}
