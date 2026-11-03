/*
 * A308726  The number of permutations of length n and tier at most 1, that
 *          is, the number of permutations of length n sortable by two passes
 *          through a stack where outputting the longest prefix matching the
 *          identity permutation is prioritized.
 * https://oeis.org/A308726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308726
   A(n) = number of permutations of length n sortable by two passes through a stack
   following a specific priority rule.
   Recurrence (conjectured D‑finite):
   3*n*(n-1)*(n+1)*a(n) -n*(n-1)*(67*n-101)*a(n-1)
   +2*(n-1)*(286*n*n-1112*n+1089)*a(n-2)
   +4*(-580*n*n*n+4200*n*n-10106*n+8049)*a(n-3)
   +24*(184*n*n*n-1784*n*n+5770*n-6221)*a(n-4)
   -96*(4*n-15)*(2*n-9)*(4*n-17)*a(n-5) = 0
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=24.
   This implementation caches computed terms and works for any seq_t size:
   • 8‑/16‑/32‑/64‑bit unsigned integers,
   • `unsigned __int128`,
   • arbitrary‑precision integer classes exported by the build (operator+, -, *, / defined). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the recurrence term for arbitrary‑precision seq_t. */
static inline seq_t
big_compute(idx_t n, const seq_t *cache)
{
    seq_t ns = (seq_t)n;
    seq_t a1 = cache[n - 1];
    seq_t a2 = cache[n - 2];
    seq_t a3 = cache[n - 3];
    seq_t a4 = cache[n - 4];
    seq_t a5 = cache[n - 5];

    seq_t c1  = (seq_t)67 * ns - 101;
    seq_t c2  = (seq_t)286 * ns * ns - (seq_t)1112 * ns + 1089;
    seq_t c3p = (seq_t)580 * ns * ns * ns - (seq_t)4200 * ns * ns
              + (seq_t)10106 * ns - 8049;
    seq_t c4  = (seq_t)184 * ns * ns * ns - (seq_t)1784 * ns * ns
              + (seq_t)5770 * ns - 6221;
    seq_t c5  = ((seq_t)4 * ns - 15) * ((seq_t)2 * ns - 9) * ((seq_t)4 * ns - 17);

    seq_t term1 = ns * (ns - 1) * c1 * a1;
    seq_t term2 = (seq_t)2 * (ns - 1) * c2 * a2;
    seq_t term3 = (seq_t)4 * c3p * a3;
    seq_t term4 = (seq_t)24 * c4 * a4;
    seq_t term5 = (seq_t)96 * c5 * a5;

    seq_t num = term1 - term2 + term3 - term4 + term5;
    seq_t den = (seq_t)3 * ns * (ns - 1) * (ns + 1);
    return num / den;
}

/* Compute the recurrence term for integer seq_t (8/16/32/64 bit or __int128). */
static inline seq_t
small_compute(idx_t n, const seq_t *cache)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long a1 = (unsigned long long)cache[n - 1];
    unsigned long long a2 = (unsigned long long)cache[n - 2];
    unsigned long long a3 = (unsigned long long)cache[n - 3];
    unsigned long long a4 = (unsigned long long)cache[n - 4];
    unsigned long long a5 = (unsigned long long)cache[n - 5];

    unsigned long long c1   = 67ULL * nn - 101ULL;
    unsigned long long c2   = 286ULL * nn * nn
                            - 1112ULL * nn
                            + 1089ULL;
    unsigned long long c3p  = 580ULL * nn * nn * nn
                            - 4200ULL * nn * nn
                            + 10106ULL * nn
                            - 8049ULL;
    unsigned long long c4   = 184ULL * nn * nn * nn
                            - 1784ULL * nn * nn
                            + 5770ULL * nn
                            - 6221ULL;
    unsigned long long c5   = (4ULL * nn - 15ULL) * (2ULL * nn - 9ULL) * (4ULL * nn - 17ULL);

    __int128 term1 = (__int128)nn * (nn - 1ULL) * c1 * a1;
    __int128 term2 = (__int128)2ULL * (nn - 1ULL) * c2 * a2;
    __int128 term3 = (__int128)4ULL * c3p * a3;
    __int128 term4 = (__int128)24ULL * c4 * a4;
    __int128 term5 = (__int128)96ULL * c5 * a5;

    __int128 numerator   = term1 - term2 + term3 - term4 + term5;
    __int128 denominator = (__int128)3ULL * nn * (nn - 1ULL) * (nn + 1ULL);

    return (seq_t)(numerator / denominator);
}

#define INITIAL_CAP 16

/* Ensure the cache array can hold indices up to n. */
static void
ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)n + 1;
    if (needed <= *cap) return;

    size_t new_cap = needed * 2;
    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (!new_arr) return;            /* allocation failure – abort */
    if (*cache) {
        for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*cache)[i];
        mem_free(*cache);
    }
    *cache = new_arr;
    *cap   = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 4;     /* a(0) … a(4) are known */

    /* Initialize cache if needed. */
    if (!cache) {
        cache_cap = INITIAL_CAP;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;    /* allocation failure – return 0 */
        cache[0] = 1; cache[1] = 1;
        cache[2] = 2; cache[3] = 6;
        cache[4] = 24;
    }

    if (n <= last_computed) return cache[n];

    ensure_cache(n, &cache, &cache_cap);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Use the small routine only for types that fit in an unsigned 64‑bit integer. */
        if (sizeof(seq_t) <= 8) {
            cache[i] = small_compute(i, cache);
        } else {
            cache[i] = big_compute(i, cache);
        }
    }
    last_computed = n;
    return cache[n];
}
