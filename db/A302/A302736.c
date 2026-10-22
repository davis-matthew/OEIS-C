/*
 * A302736  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 4 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n‑1) + 2*a(n‑2) + 2*a(n‑3) – 20*a(n‑4) – 16*a(n‑5),
 *            with a(1) = 4, a(2) = 32, a(3) = 228, a(4) = 1652, a(5) = 11980
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  a: compute OEIS sequence A302736.
    idx_t is an unsigned 64‑bit type;
    seq_t is an unsigned integer type (fixed or arbitrary precision).   */
seq_t a(idx_t n)
{
    /* 1‑based cache (cache[0] is unused, set to 0 for safety). */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* number of allocated slots          */
    static idx_t    max_n = 0;   /* highest index already computed     */

    /* Ensure the cache can hold indices up to n. */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;   /* need index n accessible        */
        if (newcap < 6)                  /* at least slots 0..5 needed      */
            newcap = 6;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();               /* memory allocation failure      */
        cache = tmp;
        cap   = newcap;
        /* zero the newly allocated part; the rest already holds values */
        for (size_t i = max_n + 1; i < newcap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Initialise the first five terms once. */
    if (max_n == 0) {
        cache[0] = (seq_t)0; /* unused slot */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)228;
        cache[4] = (seq_t)1652;
        cache[5] = (seq_t)11980;
        max_n   = 5;
    }

    /* If the requested term is already computed, return it. */
    if (n <= max_n)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        size_t ui = (size_t)i;
        /* i ≥ 6 so all indices ui‑1 … ui‑5 are valid. */
        seq_t t = (seq_t)7 * cache[ui-1];
        t += (seq_t)2 * cache[ui-2];
        t += (seq_t)2 * cache[ui-3];
        seq_t s = (seq_t)20 * cache[ui-4];
        s += (seq_t)16 * cache[ui-5];
        cache[ui] = t - s;  /* exact arithmetic in seq_t */
    }

    max_n = n;
    return cache[(size_t)n];
}
