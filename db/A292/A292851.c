/*
 * A292851  Fibonacci numbers that are also generalized octagonal numbers.
 * https://oeis.org/A292851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = Fibonacci number such that 3*a(n)+1 is a perfect square
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: full, pass, 6 terms through n = 5 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence contains each such Fibonacci number exactly once. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Static state to avoid recomputation on repeated calls */
    static bool initialized = false;
    static seq_t a_fib = 0;   /* current Fibonacci value to test (F_k) */
    static seq_t b_fib = 1;   /* next Fibonacci value (F_{k+1}) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of cached terms */
    static size_t cache_cap = 0; /* allocated capacity of cache */

    if (!initialized) {
        /* Initial values: F_0 = 0, F_1 = 1 */
        a_fib = 0;
        b_fib = 1;
        cache   = NULL;
        cache_len = 0;
        cache_cap = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n+1 elements in cache */
    while (cache_len <= n) {
        /* Check whether a_fib is a generalized octagonal number      */
        /* Condition: 3*F + 1 is a perfect square.                     */
        seq_t y = a_fib * 3 + 1;
        seq_t s = floor_sqrt(y);
        if (s * s == y) {
            /* Ensure uniqueness (skip duplicate Fibonacci values). */
            if (cache_len == 0 || cache[cache_len - 1] != a_fib) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = a_fib;
            }
        }

        /* Advance Fibonacci sequence: F_{k+2} = F_{k+1} + F_k     */
        seq_t next = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = next;
    }

    return cache[n];
}
