/*
 * A321001  Primes which contain the fax number of the beast (667).
 * https://oeis.org/A321001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 704 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes containing the decimal digit string 667.
   The function searches incrementally for the next qualifying prime
   and caches the results for future calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: test whether a decimal number contains the substring "667". */
static bool contains667(seq_t v)
{
    if (v < (seq_t)667)
        return false;

    unsigned char digits[80];            /* 80 > 39 (max digits of 128‑bit) */
    size_t dcnt = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        digits[dcnt++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    if (dcnt < 3)
        return false;

    for (size_t i = 2; i < dcnt; ++i) {
        if (digits[i] == 6 && digits[i-1] == 6 && digits[i-2] == 7)
            return true;
    }
    return false;
}

#ifdef __cplusplus
/* In C++ a cast from void* to seq_t* is required. */
#define ALLOC(sz) mem_new(seq_t, (sz) / sizeof(seq_t))
#define REALLOC(ptr, sz) mem_resize(seq_t, (ptr), (sz) / sizeof(seq_t))
#else
/* In C the void* can be assigned to any pointer type directly. */
#define ALLOC(sz) malloc(sz)
#define REALLOC(ptr, sz) realloc((void*)(ptr), sz)
#endif

/* Cached sequence data. */
static seq_t *seq_cache   = NULL;
static size_t  cache_cap  = 0;   /* allocated slots */
static idx_t   cache_len  = 0;   /* stored terms */

/* Next number to test for primality. */
static seq_t next_candidate = (seq_t)1667;

/* Ensure that seq_cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (seq_cache == NULL) {
        cache_cap = 16;
        seq_cache = ALLOC(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        seq_t candidate = next_candidate;
        while (true) {
            if (contains667(candidate) && is_prime_big(candidate)) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    seq_cache = REALLOC(seq_cache, cache_cap * sizeof(seq_t));
                }
                seq_cache[cache_len++] = candidate;
                next_candidate = candidate + (seq_t)1;
                break;
            }
            candidate++;                /* try next integer */
        }
    }
}

/* External entry point. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    ensure_terms(n);
    return seq_cache[n-1];
}
