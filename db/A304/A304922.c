/*
 * A304922  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based implementation of OEIS A304922.
   a(n) = 3*a(n-1) + 2*a(n-3) - 4*a(n-4) - 6*a(n-5) - 6*a(n-6) - a(n-7)
          + 23*a(n-8) - 12*a(n-9) + 4*a(n-10) + 8*a(n-11)
          - 2*a(n-12) - 8*a(n-13),   for n > 18
   Initial values (for 1 ≤ n ≤ 18):
   8, 9, 31, 87, 266, 851, 2510, 7348, 21910, 65581,
   195382, 581778, 1734115, 5169644, 15407810,
   45920848, 136868173, 407941876 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms, 1‑based index → index-1 */
static size_t cache_len = 0;         /* current allocated length */
static size_t cache_n = 0;           /* number of terms stored */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_len)
        return;
    size_t new_len = cache_len ? cache_len * 2 : 32;
    while (new_len < needed)
        new_len *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined offset, return 0 */
    /* initial values for n = 1 .. 18 */
    static const seq_t init[18] = {
        8, 9, 31, 87, 266, 851, 2510, 7348, 21910, 65581,
        195382, 581778, 1734115, 5169644, 15407810,
        45920848, 136868173, 407941876
    };

    if (!cache) {
        /* prime allocation and fill first 18 terms */
        size_t init_cap = 32;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i)
            cache[i] = init[i];
        cache_len = init_cap;
        cache_n = 18;
    }

    if ((size_t)n > cache_n) {
        ensure_capacity((size_t)n);
        /* compute terms from cache_n+1 up to n */
        for (idx_t idx = cache_n; idx < n; ++idx) {
            idx_t j = idx + 1; /* actual index value */
            if (j <= 18) {
                /* non‑computed in this loop; already in cache */
                continue;
            }
            /* positive part */
            seq_t pos = 0;
            pos += 3 * cache[idx - 1];
            pos += 2 * cache[idx - 3];
            pos += 23 * cache[idx - 8];
            pos += 4 * cache[idx - 10];
            pos += 8 * cache[idx - 11];
            /* negative part */
            seq_t neg = 0;
            neg += 4 * cache[idx - 4];
            neg += 6 * cache[idx - 5];
            neg += 6 * cache[idx - 6];
            neg += cache[idx - 7];
            neg += 12 * cache[idx - 9];
            neg += 2 * cache[idx - 12];
            neg += 8 * cache[idx - 13];
            cache[idx] = pos - neg;
        }
        cache_n = n;
    }
    return cache[n - 1];
}
