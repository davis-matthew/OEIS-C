/*
 * A303192  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 11*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            10*a(n-6) + 6*a(n-7) for n > 11 Initial values: a(1)=3, a(2)=9,
 *            a(3)=12, a(4)=23, a(5)=62, a(6)=122, a(7)=305, a(8)=793,
 *            a(9)=1757, a(10)=4500, a(11)=11625
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static size_t max_n = 0;      /* highest n fully computed in cache   */
static size_t cap   = 0;      /* current allocated capacity */

/* Grow the cache so that it can hold values up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= max_n) return;

    size_t new_cap = (cap ? cap : 11);  /* start with at least 11 slots */
    while (new_cap < (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
    if (cache) {
        for (idx_t i = 1; i <= max_n; ++i) tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cap   = new_cap;
    max_n = 0;   /* will be updated after base terms are initialized */
}

/* Initialise the cache with the first 11 terms. */
static void init_base(void)
{
    if (max_n > 0) return;              /* already initialised */

    cache[1]  = (seq_t)3;
    cache[2]  = (seq_t)9;
    cache[3]  = (seq_t)12;
    cache[4]  = (seq_t)23;
    cache[5]  = (seq_t)62;
    cache[6]  = (seq_t)122;
    cache[7]  = (seq_t)305;
    cache[8]  = (seq_t)793;
    cache[9]  = (seq_t)1757;
    cache[10] = (seq_t)4500;
    cache[11] = (seq_t)11625;
    max_n = 11;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* behaviour for n < 1 is arbitrary */

    ensure_cache(n);
    if (max_n == 0) init_base();          /* ensure base terms are present */

    if (n <= max_n) return cache[n];

    /* Compute terms from max_n+1 up to n using the recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val =  (seq_t)2 * cache[i - 1];
        val -= cache[i - 2];
        val += (seq_t)11 * cache[i - 3];
        val -= (seq_t)15 * cache[i - 4];
        val += (seq_t)6  * cache[i - 5];
        val -= (seq_t)10 * cache[i - 6];
        val += (seq_t)6  * cache[i - 7];
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
