/*
 * A297861  Number of nX3 0..1 arrays with every element equal to 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +4*a(n-2) -13*a(n-4) -8*a(n-5) -3*a(n-6)
 *            -4*a(n-7) +44*a(n-8) +33*a(n-9) -55*a(n-10) -42*a(n-11)
 *            +26*a(n-12) +16*a(n-13) -4*a(n-14) Empirical recurrence for
 *            n>16, with initial values a(1)=0, a(2)=2, a(3)=4, a(4)=13,
 *            a(5)=62, a(6)=222, a(7)=820, a(8)=3159, a(9)=11882,
 *            a(10)=44870,...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[16] = {
    0, 2, 4, 13, 62, 222, 820, 3159,
    11882, 44870, 169628, 640493, 2419796,
    9141186, 34529922, 130440579
};

static seq_t *cache = NULL;
static size_t cache_size = 0;      /* number of terms already computed (first term at index 0) */
static size_t cache_cap  = 0;      /* allocated capacity of the cache array */

/* Ensure that the cache has computed all terms up to n. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                      /* no value at position 0 */
    if ((size_t)n <= cache_size) return;      /* already computed */

    size_t needed = (size_t)n;

    /* Initial allocation */
    if (!cache) {
        cache_cap = needed > 16 ? needed : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;                  /* allocation failure – caller will handle */

        /* copy the known initial values */
        size_t copy_cnt = cache_cap < 16 ? cache_cap : 16;
        for (size_t i = 0; i < copy_cnt; ++i) {
            cache[i] = init_vals[i];
        }
        cache_size = copy_cnt;
    }
    /* Grow the cache if required */
    else if (needed > cache_cap) {
        size_t new_cap = needed * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                    /* allocation failure – caller will handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any newly required terms */
    for (size_t i = cache_size + 1; i <= needed; ++i) {
        size_t idx = i - 1;                    /* array index for a(i) */
        if (i <= 16) {
            cache[idx] = init_vals[idx];
        } else {
            /* recurrence with parentheses to avoid unintended signedness */
            cache[idx] =
                3 * cache[idx - 1] + 4 * cache[idx - 2]
                - (13 * cache[idx - 4]) - (8 * cache[idx - 5])
                - (3 * cache[idx - 6]) - (4 * cache[idx - 7])
                + (44 * cache[idx - 8]) + (33 * cache[idx - 9])
                - (55 * cache[idx - 10]) - (42 * cache[idx - 11])
                + (26 * cache[idx - 12]) + (16 * cache[idx - 13])
                - (4 * cache[idx - 14]);
        }
    }
    cache_size = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
