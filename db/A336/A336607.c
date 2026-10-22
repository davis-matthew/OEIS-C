/*
 * A336607  Numbers of the form x^3 + x^2*y + x*y^2 + y^3, where x and y are
 *          positive integers.
 * https://oeis.org/A336607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336607: a(n) = the n‑th distinct integer of the form
 * (x+y)(x^2 + y^2) with positive integers x, y, sorted in increasing
 * order.  The sequence starts 4, 15, 32, 40, 65, 85, … */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort (ascending order). */
static int cmp_seq_t_func(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Static storage for the computed sequence cache. */
static seq_t *cache      = NULL;   /* sorted distinct values */
static size_t cache_len  = 0;       /* number of valid entries in cache */
static idx_t  current_K  = 0;       /* current bound for x and y */

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;

    /* Cache already sufficient. */
    if (cache != NULL && cache_len >= (size_t)n)
        return;

    /* Start with a guess for K; grow until we have enough terms. */
    idx_t K = (current_K != 0) ? current_K : 200;

    while (1) {
        /* Number of pairs (x,y) with 1 <= x <= y <= K. */
        size_t total_pairs = (size_t)K * (K + 1) / 2;

        /* Allocate temporary storage. */
        seq_t *temp = (seq_t *)malloc(total_pairs * sizeof(seq_t));
        if (!temp)
            exit(1); /* Out of memory */

        /* Compute all values for the current K. */
        size_t len = 0;
        for (idx_t x = 1; x <= K; ++x) {
            seq_t sx   = (seq_t)x;
            seq_t sx2  = sx * sx;
            for (idx_t y = x; y <= K; ++y) {
                seq_t sy   = (seq_t)y;
                seq_t sy2  = sy * sy;
                seq_t val  = (sx + sy) * (sx2 + sy2);
                temp[len++] = val;
            }
        }

        /* Sort the values. */
        qsort(temp, len, sizeof(seq_t), cmp_seq_t_func);

        /* Count unique entries. */
        size_t uniq = 0;
        if (len > 0) {
            uniq = 1;
            for (size_t i = 1; i < len; ++i)
                if (temp[i] != temp[i - 1])
                    ++uniq;
        }

        /* If we have enough terms, build the final cache. */
        if (uniq >= (size_t)n) {
            /* Allocate new cache. */
            seq_t *new_cache = (seq_t *)malloc(uniq * sizeof(seq_t));
            if (!new_cache)
                exit(1);

            /* Copy unique values into the cache. */
            size_t idx = 0;
            if (len > 0) {
                new_cache[0] = temp[0];
                idx = 1;
                for (size_t i = 1; i < len; ++i) {
                    if (temp[i] != temp[i - 1]) {
                        new_cache[idx++] = temp[i];
                    }
                }
            }

            /* Replace old cache. */
            free(cache);
            cache      = new_cache;
            cache_len  = uniq;
            current_K  = K;
            free(temp);
            return;
        }

        /* Not enough; release temp and double K. */
        free(temp);
        K *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1; return 0 as placeholder */
        return (seq_t)0;

    ensure_cache(n);            /* guarantee cache has at least n terms */

    return cache[n - 1];        /* indices are 1‑based in OEIS */
}
