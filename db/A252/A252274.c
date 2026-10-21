/*
 * A252274  Number of (n+2)X(4+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 2 3 4 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum equal to 2 3 4 6 or 7.
 * https://oeis.org/A252274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-3) - 62*a(n-6) + 47*a(n-9) - 5*a(n-12) for n>15 *
 *            Initial values (n = 1 … 15): * 322, 948, 2513, 4895, 15739,
 *            43488, 85808, 276544, 765830, * 1513196, 4875644, 13503569,
 *            26685485, 85977789, 238124824
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#include <cstddef>
#define ALLOC(type, sz) reinterpret_cast<type*>(malloc(sz))
#define REALLOC(type, ptr, sz) reinterpret_cast<type*>(realloc(ptr, sz))
#else
#define ALLOC(type, sz) ((type*)malloc(sz))
#define REALLOC(type, ptr, sz) ((type*)realloc(ptr, sz))
#endif

/* Base values a(1) .. a(15) */
static const seq_t base_vals[15] = {
    322,      948,     2513,     4895,     15739,
    43488,    85808,   276544,   765830,   1513196,
   4875644, 13503569, 26685485, 85977789,238124824
};

/* Memoisation cache: 0‑indexed (a(1) at cache[0]) */
static seq_t *cache = NULL;   /* pointer to cached values */
static size_t   cap   = 0;    /* allocated capacity (in elements) */
static size_t   sz    = 0;    /* number of values currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    size_t N = (size_t)n;   /* cast to size_t for indexing */

    /* Handle small n directly */
    if (N <= 15) {
        return base_vals[N-1];
    }

    /* Initialise or grow cache */
    if (cache == NULL) {
        cap = (N > 15) ? N : 15;
        cache = ALLOC(seq_t, cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory; undefined behaviour */
        for (size_t i = 0; i < 15; ++i) cache[i] = base_vals[i];
        sz = 15;
    } else if (cap < N) {
        size_t new_cap = cap * 2;
        if (new_cap < N) new_cap = N;
        seq_t *tmp = REALLOC(seq_t, cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* If the requested value is already computed, return it */
    if (N <= sz) return cache[N-1];

    /* Compute values up to N (inclusive) */
    for (size_t i = sz; i < N; ++i) {
        /* i corresponds to a(i+1) */
        /* Recurrence uses a(n-3), a(n-6), a(n-9), a(n-12) */
        seq_t val = 21 * cache[i-3];
        val -= 62 * cache[i-6];
        val += 47 * cache[i-9];
        val -= 5  * cache[i-12];
        cache[i] = val;
    }
    sz = N;
    return cache[N-1];
}
