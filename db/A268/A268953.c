/*
 * A268953  Number of length-n 0..5 arrays with no repeated value equal to the
 *          previous repeated value, with new values introduced in sequential
 *          order.
 * https://oeis.org/A268953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined for n ≥ 1 by the initial values
 *            a(1)=1,a(2)=2,a(3)=4,a(4)=12,a(5)=40,a(6)=154,a(7)=655,a(8)=3045,
 *            a(9)=15124,a(10)=78930,a(11)=426289,a(12)=2355975,a(13)=13216982,
 *            a(14)=74856340, and for n>14 by the recurrence a(n) = 18 a(n‑1)
 *            – 110 a(n‑2) + 170 a(n‑3) + 801 a(n‑4) – 2920 a(n‑5)...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises the values so that subsequent calls are O(1). */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based index; return 0 if n < 1 (offset behaviour is irrelevant) */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* static memoisation table */
    static seq_t *cache       = NULL;
    static size_t cache_len  = 0;   /* number of terms stored in cache */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */

    /* On first call, allocate space and store the 14 known terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            abort();

        static const seq_t init[14] = {
            (seq_t)1,   (seq_t)2,   (seq_t)4,   (seq_t)12,
            (seq_t)40,  (seq_t)154, (seq_t)655, (seq_t)3045,
            (seq_t)15124,(seq_t)78930,(seq_t)426289,(seq_t)2355975,
            (seq_t)13216982,(seq_t)74856340
        };

        for (size_t i = 0; i < 14; ++i)
            cache[i] = init[i];
        cache_len = 14;
    }

    /* Ensure the cache is large enough for n */
    size_t N = (size_t)n;
    if (N > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < N)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to N */
    if (N > cache_len) {
        for (size_t idx = cache_len; idx < N; ++idx) {
            /* Apply recurrence for n = idx+1; n>14 ⇔ idx≥14 */
            if (idx >= 14) {
                seq_t val = 0;
                /* a(n) = 18*a(n‑1) – 110*a(n‑2) + 170*a(n‑3) + 801*a(n‑4)
                          – 2920*a(n‑5) – 662*a(n‑6) + 12700*a(n‑7)
                          – 6980*a(n‑8) – 22928*a(n‑9) + 19192*a(n‑10)
                          + 15120*a(n‑11) – 14400*a(n‑12) */
                val += cache[idx-1] * (seq_t)18;
                val -= cache[idx-2] * (seq_t)110;
                val += cache[idx-3] * (seq_t)170;
                val += cache[idx-4] * (seq_t)801;
                val -= cache[idx-5] * (seq_t)2920;
                val -= cache[idx-6] * (seq_t)662;
                val += cache[idx-7] * (seq_t)12700;
                val -= cache[idx-8] * (seq_t)6980;
                val -= cache[idx-9] * (seq_t)22928;
                val += cache[idx-10] * (seq_t)19192;
                val += cache[idx-11] * (seq_t)15120;
                val -= cache[idx-12] * (seq_t)14400;
                cache[idx] = val;
            } else { /* idx < 14 : these already stored in init */
                /* nothing to do */
                continue;
            }
        }
        cache_len = N;
    }

    return cache[N-1];
}
