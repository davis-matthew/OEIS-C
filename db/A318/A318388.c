/*
 * A318388  a(1) = 1, a(n) = -floor(e*a(n/2)) if n is even, a(n) = n - a(n-1)
 *          if n is odd.
 * https://oeis.org/A318388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; a(n) = -floor(e * a(n/2)) if n is even, a(n) = n -
 *            a(n-1) if n is odd
 * Range:     i8 n <= 27, i16 n <= 973, i32 n <= 9885, i64 n <= 9885,
 *            i128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* Fractional approximation of e: e ≈ 2718281828459045 / 1000000000000000
   (accurate to 15 fp digits).  The large denominator keeps the
   intermediate product within 128‑bit bounds while maintaining the
   required precision. */
static const uint64_t E_NUM = 2718281828459045ULL;   /* 2.718281828459045…×10¹⁵ */
static const uint64_t E_DEN = 1000000000000000ULL;   /* 10¹⁵          */

/* Floor(e * a) with correct handling of negative values.
   Uses 128‑bit arithmetic to avoid overflow. */
static inline seq_t floor_e_mul(seq_t a)
{
    long long aa = (long long)a;                     /* value is small, safe to cast */
    __int128 prod = (__int128)aa * (__int128)E_NUM; /* 128‑bit product takes care of overflow */
    if (prod >= 0)
        return (seq_t)(prod / (__int128)E_DEN);
    else
        /* truncation toward zero → adjust so that ÷ gives the floor */
        return (seq_t)((prod - (__int128)(E_DEN - 1)) / (__int128)E_DEN);
}

/* Cache for memoised values.  cache[0] is unused; indices start at 1. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t max_computed = 0;

static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();            /* unrecoverable allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }
}

/* Computes a(n) with memoisation, supporting any order of calls. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* defined only for n ≥ 1 */
    ensure_cache(n);

    /* Initialise base case if necessary. */
    if (max_computed == 0) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute all missing terms up to n. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if ((i & 1) == 0) {          /* even */
                cache[i] = -floor_e_mul(cache[i / 2]);
            } else {                     /* odd */
                cache[i] = (seq_t)i - cache[i - 1];
            }
        }
        max_computed = n;
    }
    return cache[n];
}
