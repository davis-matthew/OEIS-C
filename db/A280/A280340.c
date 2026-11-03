/*
 * A280340  a(n) = a(n-1) + 10^n * a(n-2) with a(0) = 1 and a(1) = 1.
 * https://oeis.org/A280340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10^n * a(n-2), with a(0) = 1, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 59 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* size_t */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *cache = NULL;   /* cache[0..max_cached-1] */
static size_t max_cached = 0; /* number of terms already computed */
static size_t cap = 0;        /* capacity of cache */

/* Return the n-th term of sequence A280340. */
seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* Ensure space for up to n. */
    size_t need = (size_t)n + 1;
    if (!cache || need > cap)
        grow_seq(&cache, &cap, need);

    /* Initialise base terms if this is the first computation. */
    if (max_cached < 2) {
        cache[0] = 1;
        cache[1] = 1;
        max_cached = 2;
    }

    /* Compute missing terms up to n.  Use a running power of 10. */
    seq_t pow10 = ipow_idx((seq_t)10, (idx_t)(max_cached - 1));

    for (idx_t i = (idx_t)max_cached; i <= n; ++i) {
        /* Update pow10 to 10^i. */
        pow10 = mul_small(pow10, 10u);               /* 10^{i-1} * 10 = 10^i */

        /* Compute a(i) = a(i-1) + 10^i * a(i-2). */
        seq_t prod = seq_mul(pow10, cache[i - 2]);   /* 10^i * a(i-2) */
        cache[i] = cache[i - 1] + prod;              /* a(i-1) + prod */

        ++max_cached;
    }

    return cache[n];
}
