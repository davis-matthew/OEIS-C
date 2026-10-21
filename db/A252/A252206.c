/*
 * A252206  Number of (n+2)X(4+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum equal to 1 3 4 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum not equal to 1 3 4 6 or 7.
 * https://oeis.org/A252206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 260*a(n-3) -1023*a(n-6) -260*a(n-9) +1024*a(n-12) (n >
 *            15) with initial values a(1) … a(15) given in the OEIS entry
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation notes:
    * Cache up to the largest n requested; store the first 15 terms
      explicitly and compute further values by the above recurrence.
      The two large terms are added first, then the two subtracted
      terms are added and subtracted from that sum, guaranteeing
      no unsigned underflow for n > 15.
    * The array is allocated with malloc/calloc and casted to (seq_t*)
      to keep the code valid in both C11 and C++17.
    * No global state other than the cache.  The function can be
      called in any order.  It runs in O(n) time, which is easily
      fast enough for n ≈ 10 000.  Standard headers only. */

#include <stdlib.h>
#include <stdint.h>

/* static cache: 1‑based index, cache_len is the largest computed index */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure cache has space for indices up to n (inclusive) and
   initialise the first 15 terms if this is the first call. */
static void ensure_capacity(size_t n)
{
    if (cache == NULL) {
        /* Allocate space for indices 0..15 (0 unused). */
        size_t cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();

        /* initialise known values (a(1) … a(15)) */
        cache[1]  = (seq_t)19794ULL;
        cache[2]  = (seq_t)44322ULL;
        cache[3]  = (seq_t)332462ULL;
        cache[4]  = (seq_t)1180226ULL;
        cache[5]  = (seq_t)11207238ULL;
        cache[6]  = (seq_t)84935426ULL;
        cache[7]  = (seq_t)301990806ULL;
        cache[8]  = (seq_t)2868905558ULL;
        cache[9]  = (seq_t)21743274306ULL;
        cache[10] = (seq_t)77309414066ULL;
        cache[11] = (seq_t)734439412518ULL;
        cache[12] = (seq_t)5566277622866ULL;
        cache[13] = (seq_t)19791209309526ULL;
        cache[14] = (seq_t)188016488368598ULL;
        cache[15] = (seq_t)1424967069625986ULL;

        cache_len = 15;
    }

    if (n > cache_len) {
        /* Grow the array.  Double until it can hold n. */
        size_t new_cap = cache_len ? cache_len * 2 : 32;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 */

    size_t idx = (size_t)n;
    ensure_capacity(idx);

    if (idx > cache_len) {
        /* compute missing terms in ascending order */
        for (size_t k = cache_len + 1; k <= idx; ++k) {
            /* recurrence only applies for k > 15 */
            if (k <= 15) continue;        /* already initialised */

            seq_t t1 = (seq_t)260u * cache[k-3];
            seq_t t2 = (seq_t)1023u * cache[k-6];
            seq_t t3 = (seq_t)260u * cache[k-9];
            seq_t t4 = (seq_t)1024u * cache[k-12];

            /* sum of the positive terms first, then subtract the
               sum of the negative terms – guarantees no unsigned underflow */
            seq_t sum_top    = t1 + t4;
            seq_t sum_bottom = t2 + t3;
            cache[k] = sum_top - sum_bottom;
        }
        cache_len = idx;
    }

    return cache[idx];
}
