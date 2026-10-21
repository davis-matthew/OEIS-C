/*
 * A252012  Number of (n+2)X(5+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum 1 3 6 or 8 and every diagonal and antidiagonal sum not
 *          1 3 6 or 8.
 * https://oeis.org/A252012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 45, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 21*a(n-3) - 21*a(n-4) - 104*a(n-6) + 104*a(n-7)
   for n > 16.  Initial values (1‑based):
   a(1)=1970, a(2)=3128, a(3)=4770, a(4)=8482, a(5)=23074,
   a(6)=58970, a(7)=104034, a(8)=288370, a(9)=744818,
   a(10)=1308322, a(11)=3648082, a(12)=9501482, a(13)=16654562,
   a(14)=46626322, a(15)=122075402, a(16)=213680482 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- static cache for memoisation ---------- */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;                /* number of allocated slots */

    /* ---------- initialise cache on first call ---------- */
    if (cache_size == 0) {
        static const seq_t init_vals[17] = {
            0ULL,                           /* a(0) – unused, index 1‑based */
            1970ULL, 3128ULL, 4770ULL, 8482ULL, 23074ULL,
            58970ULL, 104034ULL, 288370ULL, 744818ULL,
            1308322ULL, 3648082ULL, 9501482ULL, 16654562ULL,
            46626322ULL, 122075402ULL, 213680482ULL
        };
        cache_size = 17;                     /* indices 0 … 16 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                 /* OOM → hard fail */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* ---------- make sure we have space for index n ---------- */
    if ((idx_t)n + 1 > cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                  /* OOM → hard fail */
        /* initialise newly allocated slots */
        for (idx_t i = old_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
        /* we will start computing from the first uninitialised index */
        old_size = cache_size - (new_size - old_size); /* restore old_size */
        /* but we still have old_size stored above; use it */
        idx_t start = old_size;            /* first index not yet computed */
        /* compute missing terms using the recurrence */
        for (idx_t i = start; i <= n; ++i) {
            seq_t term = cache[i - 1];
            term += (seq_t)21 * cache[i - 3];
            term -= (seq_t)21 * cache[i - 4];
            term -= (seq_t)104 * cache[i - 6];
            term += (seq_t)104 * cache[i - 7];
            cache[i] = term;
        }
        return cache[(size_t)n];
    }

    /* ---------- compute only if necessary ---------- */
    if (n <= 16)                      /* already known */
        return cache[(size_t)n];

    idx_t start = cache_size;          /* first uncomputed index */
    for (idx_t i = start; i <= n; ++i) {
        seq_t term = cache[i - 1];
        term += (seq_t)21 * cache[i - 3];
        term -= (seq_t)21 * cache[i - 4];
        term -= (seq_t)104 * cache[i - 6];
        term += (seq_t)104 * cache[i - 7];
        cache[i] = term;
    }
    return cache[(size_t)n];
}
