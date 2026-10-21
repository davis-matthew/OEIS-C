/*
 * A267351  Decimal representation of the n-th iteration of the "Rule 123"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-2) - 84*a(n-4) + 64*a(n-6) (n>6) Initial values:
 *            a(0)=1, a(1)=5, a(2)=14, a(3)=119, a(4)=56, a(5)=2015, a(6)=224
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to compute a recurrence term safely.
   If seq_t is 64‑bit or smaller, use a 128‑bit intermediate
   to avoid overflow from the 64‑bit multiplication. */
static inline seq_t compute_term(seq_t a2, seq_t a4, seq_t a6)
{
    /* a2: a(n-2), a4: a(n-4), a6: a(n-6) */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)a2 * 21;
        res += (unsigned __int128)a6 * 64;
        res -= (unsigned __int128)a4 * 84;
        return (seq_t)res;
    } else {
        /* seq_t is 128‑bit or a big‑integer type; native operations suffice */
        return a2 * 21 + a6 * 64 - a4 * 84;
    }
}

/* Cached array of computed terms.  The array grows as larger
   indices are required.  All earlier values are preserved. */
static seq_t *cache   = NULL;   /* pointer to array of seq_t */
static idx_t cache_cap = 0;     /* number of allocated slots (>=max_index+1) */

seq_t a(idx_t n)
{
    /* Ensure cache has space for indices 0 … n */
    if (n + 1 > cache_cap) {
        /* New size, grow 1.5× for amortized linear growth */
        size_t new_cap = n + 1;
        if (new_cap < 64) new_cap = 64;
        size_t grow_factor = 3;
        while (new_cap * 2 < (n + 1) * grow_factor) {
            new_cap = new_cap * grow_factor / 2;
        }
        if (new_cap < n + 1) new_cap = n + 1;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – give up; application may abort */
            return (seq_t)0;
        }
        /* Copy existing values */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* Initialize any new slots to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        /* Free old cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Base cases – fill in if not already present */
    if (n == 0) { cache[0] = (seq_t)1; return cache[0]; }
    if (n == 1) { cache[1] = (seq_t)5; return cache[1]; }
    if (n == 2) { cache[2] = (seq_t)14; return cache[2]; }
    if (n == 3) { cache[3] = (seq_t)119; return cache[3]; }
    if (n == 4) { cache[4] = (seq_t)56; return cache[4]; }
    if (n == 5) { cache[5] = (seq_t)2015; return cache[5]; }
    if (n == 6) { cache[6] = (seq_t)224; return cache[6]; }

    /* Determine starting point for new computations */
    idx_t start = 0;
    /* Find the highest index already computed */
    if (cache_cap > 0) {
        /* Scan for the largest index <= n that is already set.
           Since we always fill consecutively, it's simply the largest
           index less than n that has a non‑zero value. */
        start = 0;
        for (idx_t i = 0; i <= n; ++i) {
            if (cache[i] != (seq_t)0) start = i;
        }
    }
    if (start < 6) start = 6;          /* we need at least a(6) to start recurrence */
    if (start >= n) return cache[n];    /* already computed */

    /* Compute missing values up to n */
    for (idx_t i = start + 1; i <= n; ++i) {
        seq_t a2 = cache[i - 2];
        seq_t a4 = cache[i - 4];
        seq_t a6 = cache[i - 6];
        cache[i] = compute_term(a2, a4, a6);
    }
    return cache[n];
}
