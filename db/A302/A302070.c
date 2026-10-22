/*
 * A302070  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 4*a(n‑1) + 4*a(n‑2) + 2*a(n‑3) + 7*a(n‑4)
 *            + 7*a(n‑5) + 2*a(n‑11)+8*a(n‑12)+a(n‑13) –
 *            [4*a(n‑6)+10*a(n‑7)+3*a(n‑8)+5*a(n‑9)+11*a(n‑10)+a(n‑15)] for n
 *            > 18. Initial terms for n = 1 … 18 are given in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* 0 is undefined, return 0 for safety */

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated indices 0 … cache_cap – 1 */
    static idx_t cache_filled = 0;/* largest index that has a value */

    /* Initialise cache with the first 18 values (index 0 unused) */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        static const seq_t init_vals[19] = {
            0ULL,                  /* unused slot 0 */
            4ULL, 32ULL, 148ULL, 748ULL,
            3657ULL, 18108ULL, 89658ULL,
            444359ULL, 2202106ULL, 10912859ULL,
            54080195ULL, 268002332ULL, 1328125172ULL,
            6581720890ULL, 32616691234ULL,
            161636836900ULL, 801015249307ULL,
            3969549529089ULL
        };
        for (idx_t i = 1; i <= 18; ++i) cache[i] = init_vals[i];
        cache_filled = 18;
    }

    /* Ensure enough space for requested index */
    if (n > cache_cap - 1) {
        idx_t needed = n + 1;
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute any missing values up to n */
    if (n > cache_filled) {
        for (idx_t k = cache_filled + 1; k <= n; ++k) {
            /* The recurrence applies only for k > 18, which is guaranteed
               because cache_filled is initialised to 18. */
            seq_t pos = 4 * cache[k-1] + 4 * cache[k-2]
                        + 2 * cache[k-3] + 7 * cache[k-4]
                        + 7 * cache[k-5];
            pos += 2 * cache[k-11] + 8 * cache[k-12] + cache[k-13];

            seq_t neg = 4 * cache[k-6] + 10 * cache[k-7]
                        + 3 * cache[k-8] + 5 * cache[k-9]
                        + 11 * cache[k-10];
            neg += cache[k-15];

            cache[k] = pos - neg;   /* pos >= neg for all k */
        }
        cache_filled = n;
    }

    return cache[n];
}
