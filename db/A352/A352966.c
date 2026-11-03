/*
 * A352966  Primes of the form A321513(k) + 1 for some k > 0.
 * https://oeis.org/A352966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes OEIS A352966: primes p such that p-1, when written
 * in base‑4, can be padded with leading zeros so that the resulting string
 * is periodic with at least two identical repetitions. */

/* Needed utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Test whether base‑4 expansion of x (non‑negative) can be padded
 * with leading zeros to become a periodic string with at least two full
 * repetitions. */
static bool has_periodic(seq_t x)
{
    if (x == 0)                /* 0 → "0", only one repetition → reject */
        return false;

    /* Convert x to base‑4 digits (least‑significant first). */
    uint8_t rev[256];
    size_t len = 0;
    seq_t t = x;
    while (t != 0) {
        rev[len++] = (uint8_t)(t % 4);
        t /= 4;
    }
    if (len == 0)              /* should not happen */
        rev[len++] = 0;

    /* Try every possible period length m. */
    for (size_t m = 1; m <= len; ++m) {
        size_t L = ((len + m - 1) / m) * m;   /* smallest multiple of m ≥ len */
        size_t shift = L - len;               /* leading zeros to pad */
        size_t trep = L / m;                  /* number of repetitions */

        if (trep < 2)        /* need at least two repetitions */
            continue;

        bool ok = true;
        for (size_t i = 0; i < L - m; ++i) {
            uint8_t di = (i < shift) ? 0 : rev[len - 1 - (i - shift)];
            size_t j = i + m;
            uint8_t dj = (j < shift) ? 0 : rev[len - 1 - (j - shift)];
            if (di != dj) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

/* Cache for memoised sequence values (1‑based indexing). */
static seq_t *cache      = NULL;   /* array of found primes            */
static size_t cache_cap  = 0;      /* current allocated capacity       */
static idx_t  cache_len  = 0;      /* number of stored primes in cache */
static idx_t  next_start = 2;      /* next integer to test for prime  */

/* Grow the cache array when it is full. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    } else {
        size_t old_cap = cache_cap;
        cache_cap *= 2;
        cache = seq_realloc(cache, old_cap, cache_cap);
    }
}

/* Return the n‑th term of the sequence (a(1) = 11).  This routine
 * works for arbitrary‑precision seq_t as well as for built‑in integer
 * types. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n = 0; optional */

    /* Generate terms until we have at least n entries. */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_start++;   /* candidate integer to test */

        if (!is_prime_big(cand))
            continue;                 /* skip composites */

        if (has_periodic(cand - 1)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = cand;  /* store prime */
        }
    }
    return cache[n - 1];
}
