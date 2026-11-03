/*
 * A356498  Primes p such that 100*p + 11 is also prime.
 * https://oeis.org/A356498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 100*p + 11 is also prime. */
/* The function memoises found primes across calls. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;           /* array of qualifying primes */
static size_t cap   = 0;              /* current capacity */
static size_t len   = 0;              /* number of stored primes */
static uint64_t last_checked = 0;     /* last candidate prime examined */

/* Ensure the cache can hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp;
        if (cap == 0) {
            tmp = seq_alloc(new_cap);
        } else {
            tmp = seq_realloc(cache, cap, new_cap);
        }
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }
}

/* Find the next qualifying prime and append it to the cache. */
static void advance_cache(void)
{
    for (;;) {
        uint64_t cand = next_prime(last_checked);  /* next prime after last_checked */
        seq_t extended = ((seq_t)cand) * 100 + 11;
        if (is_prime_big(extended)) {
            grow_cache(len + 1);
            cache[len++] = (seq_t)cand;
            last_checked = cand;
            break;
        }
        /* If cand fails, continue searching from this prime */
        last_checked = cand;
    }
}

/* Return the n‑th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence offset is 1 */
    while (len < n) advance_cache();
    return cache[n - 1];
}
