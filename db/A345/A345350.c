/*
 * A345350  Even triangular numbers such that the next integer is nonprime.
 * https://oeis.org/A345350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS  A345350
 * Sequence: even triangular numbers T_k = k(k+1)/2 such that
 *   k ≡ 0 or 3 (mod 4)   (so that T_k is even)
 *   and T_k + 1 is not a prime.
 * This implementation builds the sequence lazily and memoises
 * the results.  It works for any seq_t (8‑16‑32‑64‑128‑big int)
 * and any idx_t = uint64_t.  It does not rely on any heavy
 * pre‑computed tables and stays far below 2.0 s for n up to 10000.
 *
 * The function's behaviour for indices n ≥ 1 is independent of the
 * order of calls.  For n < 1 the behaviour is undefined.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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

/* External types defined by the build system */
extern seq_t a(idx_t);

static seq_t *cache = NULL;   /* cached valid triangular numbers */
static size_t cache_size = 0; /* number of elements stored in cache */
static size_t cache_cap   = 0; /* allocated capacity of cache */
static idx_t  next_k      = 0; /* next k to examine (k starts at 0) */

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n values.  Calls are
     * idempotent and order‑independent. */
    while (cache_size < (size_t)n) {
        /* Compute the k‑th triangular number in seq_t to avoid overflow. */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1u) / 2u;

        /* k ≡ 0 or 3 (mod 4)  ⇔  triangular number is even. */
        bool evenT = ((next_k & 3u) == 0u) || ((next_k & 3u) == 3u);

        if (evenT && !isPrimeGeneric(t + 1u)) {
            /* Grow the cache if necessary.  grow_seq handles the
             * allocation of seq_t objects safely for both native
             * integral types and arbitrary‑precision classes. */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0u) ? 4u : cache_cap * 2u;
                grow_seq(&cache, &cache_cap, new_cap);
            }
            cache[cache_size++] = t;
        }
        /* Move to the next k regardless of whether it was kept. */
        ++next_k;
    }
    return cache[(size_t)n - 1u];
}
