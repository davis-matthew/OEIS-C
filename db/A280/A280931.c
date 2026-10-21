/*
 * A280931  a(n) = 2*F(n-1) + 9*F(n-4) + 9*F(n-7) where n >= 7 and F =
 *          A000045.
 * https://oeis.org/A280931
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*F(n-1) + 9*F(n-4) + 9*F(n-7), n≥7 Equivalently, a(n) =
 *            a(n-1)+a(n-2) with a(7)=34, a(8)=62 (generate using the ordinary
 *            Fibonacci recurrence)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed a(n) values */
static idx_t cache_len = 0;        /* allocated size of cache */
static idx_t highest_computed = 6; /* highest n for which a(n) is known */

/* Ensure a(n) values are computed up to at least n */
static void ensure_to(idx_t n) {
    if (n <= highest_computed) return;       /* nothing to do */

    /* enlarge storage if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* compute missing values */
    for (idx_t k = highest_computed + 1; k <= n; ++k) {
        if (k == 7) {
            cache[7] = (seq_t)34;
        } else if (k == 8) {
            cache[8] = (seq_t)62;
        } else {
            cache[k] = cache[k-1] + cache[k-2];
        }
    }
    highest_computed = n;
}

/* public function requested by the problem statement */
seq_t a(idx_t n) {
    if (n < 7) return 0;          /* behaviour below offset irrelevant */
    ensure_to(n);
    return cache[n];
}
