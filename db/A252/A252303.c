/*
 * A252303  Number of (n+2)X(6+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-3) -70*a(n-6) +120*a(n-9) -64*a(n-12) for n>21
 *            initial values (offset 1): a(1)=7138, a(2)=5056, a(3)=6103,
 *            a(4)=8130, a(5)=15444, a(6)=24076, a(7)=33862, a(8)=71636,
 *            a(9)=117612, a(10)=167798, a(11)=385332, a(12)=662964,
 *            a(13)=951766, a(14)=2359892, a(15)=4219348, a(16)=6078806,
 *            a(1...
 * Range:     u8 none, u16 n <= 7, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cached sequence values (0‑based) */
static size_t cache_len = 0;     /* number of values currently stored */
static size_t cache_cap = 0;     /* capacity of the cache */

/* Ensure the cache can hold at least `n` elements */
static void ensure_capacity(size_t n) {
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for 0, return 0 */

    /* Initialize the cache with the first 21 known values on first call. */
    if (cache_len == 0) {
        ensure_capacity(21);
        static const seq_t base_vals[21] = {
            7138ULL, 5056ULL, 6103ULL, 8130ULL, 15444ULL, 24076ULL,
            33862ULL, 71636ULL, 117612ULL, 167798ULL, 385332ULL,
            662964ULL, 951766ULL, 2359892ULL, 4219348ULL, 6078806ULL,
            16019028ULL, 29452116ULL, 42525526ULL, 116755796ULL,
            218457940ULL
        };
        for (size_t i = 0; i < 21; ++i) cache[i] = base_vals[i];
        cache_len = 21;
    }

    /* If the requested index is already cached, return it. */
    if ((idx_t)cache_len >= n) return cache[n - 1];

    /* Compute values from cache_len+1 up to n using the recurrence. */
    ensure_capacity((size_t)n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Indices are 1‑based in the recurrence, 0‑based in the array. */
        seq_t a3  = cache[(i - 3) - 1];
        seq_t a6  = cache[(i - 6) - 1];
        seq_t a9  = cache[(i - 9) - 1];
        seq_t a12 = cache[(i - 12) - 1];

        /* Compute 15*a3 + 120*a9  minus  (70*a6 + 64*a12). */
        seq_t top = (15 * a3) + (120 * a9);
        seq_t sub = (70 * a6) + (64 * a12);
        cache[i - 1] = top - sub;    /* guaranteed non‑negative */
    }
    cache_len = n;

    return cache[n - 1];
}
