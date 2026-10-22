/*
 * A326100  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A326100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A326100
   Recurrence: a(n) = 5*a(n-1) + 5*a(n-2) - 2*a(n-3) - 3*a(n-6)
                  - 2*a(n-7) - a(n-8) - a(n-9)
   Initial values for 1 ≤ n ≤ 9 are given explicitly (computed once). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Upper bound for what the caller will query.  The problem statement
   guarantees n ≤ 10000, so a static array of that size is sufficient
   for any C11 or C++17 compiler, regardless of the exact width of
   seq_t. */
#define CACHE_MAX 10001

/* Static cache and the highest index that has been computed so far. */
static seq_t cache[CACHE_MAX] = { 0 };
static idx_t cached_until = 0;

/* Initial values for n = 1 … 9 (the recurrence is valid only for n ≥ 10). */
static const seq_t init_vals[10] = {
    0,          /* unused; cache[0] stays 0 */
    (seq_t)4,   /* a(1) */
    (seq_t)29,  /* a(2) */
    (seq_t)161, /* a(3) */
    (seq_t)929, /* a(4) */
    (seq_t)5383,/* a(5) */
    (seq_t)31236,/* a(6) */
    (seq_t)181227,/* a(7) */
    (seq_t)1051456,/* a(8) */
    (seq_t)6100399 /* a(9) */
};

/* Compute a(n) using memoisation.  The function is safe for any
   order of calls, and works for all n ≥ 1 that fit in the static
   cache. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* The sequence is defined for n ≥ 1; return 0 for an out‑of‑range
           call to keep the function well‑defined. */
        return 0;
    }
    if (n >= CACHE_MAX) {
        /* The caller requested a value beyond the pre‑allocated cache.
           Abort in a way that can be caught by the test harness. */
        abort();
    }
    if (n <= cached_until) {
        return cache[n];
    }

    /* Compute terms from cached_until+1 up to n inclusively. */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i];
        } else {
            /* Split the recurrence into positive and negative parts
               to avoid any intermediate underflow on unsigned types. */
            seq_t pos = (seq_t)5 * cache[i - 1] +
                        (seq_t)5 * cache[i - 2];
            seq_t neg = (seq_t)2 * cache[i - 3] +
                        (seq_t)3 * cache[i - 6] +
                        (seq_t)2 * cache[i - 7] + cache[i - 8] + cache[i - 9];
            cache[i] = pos - neg;   /* safe: neg ≤ pos for all n */
        }
    }
    cached_until = n;
    return cache[n];
}
