/*
 * A247162  G.f. 1-(2*x^3)/(-sqrt(-3*x^4-4*x^3-2*x^2+1)-x^2+1).
 * https://oeis.org/A247162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   mul_seq_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247162
   Recurrence (valid for n ≥ 5):
       n*a(n) = -a(n-1) + (2n-5)*a(n-2) + (4n-17)*a(n-3) + 3(n-5)*a(n-4)
   Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=0.
   The implementation stores previously computed terms in a static cache
   so that calls with arbitrary n are answered in O(n) time overall.
   It uses only integer arithmetic and works with any build that supplies
   a seq_t type supporting + - * / and cast to uint64_t for small values.
   For built‑in integer types it performs multiplications via an
   explicit 128‑bit temporary when available to prevent overflow of
   intermediate results.
*/
#include <stdlib.h>
#include "../utils/mul_seq_uint.h"

static seq_t *cache = NULL;   /* 1‑based array of terms */
static size_t cache_cap = 0;  /* capacity of cache (number of usable slots) */
static idx_t max_comp = 0;    /* largest n that has already been computed */

/* ------------------------------------------------------------------ */
/* Ensure the static cache has room for indices up to n (inclusive).   */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n + 1)   /* +1 because we use 1‑based indices */
        return;

    size_t new_cap = (size_t)n + 1;   /* exact capacity needed */
    if (new_cap < 10)                 /* avoid too small allocations */
        new_cap = 10;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                      /* out of memory */

    /* Zero‑initialize any new entries for safety (not strictly required) */
    if (new_cap > cache_cap)
        for (size_t i = cache_cap; i < new_cap; ++i)
            new_ptr[i] = (seq_t)0;

    cache      = new_ptr;
    cache_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Multiply a seq_t by a small unsigned integer safely.               */
/* Uses built‑in 128‑bit arithmetic when available.                   */

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Return the trivial initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)0;

    /* Ensure the cache contains at least the first four terms. */
    if (max_comp < 4) {
        ensure_capacity(4);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;
        max_comp = 4;
    }

    /* If the requested n is already computed, return it immediately. */
    if (n <= max_comp)
        return cache[n];

    /* Compute any missing terms up to n (inclusive). */
    ensure_capacity(n);
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        seq_t a1 = cache[k - 1];
        seq_t a2 = cache[k - 2];
        seq_t a3 = cache[k - 3];
        seq_t a4 = cache[k - 4];

        uint64_t c1 = 2 * k - 5;
        uint64_t c2 = 4 * k - 17;
        uint64_t c3 = 3 * (k - 5);

        /* Compute the numerator of the recurrence. */
        seq_t sum = mul_seq_uint(a2, c1);
        sum += mul_seq_uint(a3, c2);
        sum += mul_seq_uint(a4, c3);
        sum -= a1;            /* subtraction is safe because the result is non‑negative */

        cache[k] = sum / (seq_t)k;   /* division is exact by the recurrence */
    }

    max_comp = n;
    return cache[n];
}
