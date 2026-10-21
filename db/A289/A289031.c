/*
 * A289031  Number of perfect matchings on n+3 edges which represent RNA
 *          secondary folding structures characterized by the Reeder and
 *          Giegerich and the Lyngso and Pedersen families, but not the family
 *          characterized by Cao and Chen.
 * https://oeis.org/A289031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   D-finite recurrence (n-1)*(n+5)*a(n) = 2*(3*n^2+9*n-2)*a(n-1) -
 *            (3*n+1)*(3*n+7)*a(n-2) + 2*(n+1)*(2*n+3)*a(n-3) with a(1)=1,
 *            a(2)=8, a(3)=42
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cache[1..max] holds a(1)..a(max)   */
    static size_t capacity = 0;
    static size_t max_computed = 0;   /* highest n filled in cache */

    if (n < 1) return 0;          /* undefined for n<1, but safe return */

    /* Ensure cache is large enough for index n */
    if (capacity < (size_t)(n + 2)) {          /* +2 for 1‑based & sentinel */
        size_t new_cap = capacity ? capacity * 2 : 64;
        while (new_cap < (size_t)(n + 2))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        if (cache) {
            memcpy(tmp, cache, capacity * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i == 1)          cache[1] = (seq_t)1;
        else if (i == 2)     cache[2] = (seq_t)8;
        else if (i == 3)     cache[3] = (seq_t)42;
        else {
            seq_t a_n1 = cache[i - 1];
            seq_t a_n2 = cache[i - 2];
            seq_t a_n3 = cache[i - 3];

            idx_t nn = (idx_t)i;          /* for clarity */

            /* Compute terms in 128‑bit to avoid overflow (works on GCC/Clang) */
            unsigned __int128 k1 = (unsigned __int128)2 *
                                    ((unsigned __int128)3 * nn * nn +
                                     (unsigned __int128)9 * nn -
                                     (unsigned __int128)2);
            unsigned __int128 k2 = (unsigned __int128)(3 * nn + 1) *
                                    (unsigned __int128)(3 * nn + 7);
            unsigned __int128 k3 = (unsigned __int128)2 *
                                    ((unsigned __int128)(nn + 1)) *
                                    ((unsigned __int128)(2 * nn + 3));

            unsigned __int128 num1 = k1 * (unsigned __int128)a_n1;
            unsigned __int128 num2 = k2 * (unsigned __int128)a_n2;
            unsigned __int128 num3 = k3 * (unsigned __int128)a_n3;
            unsigned __int128 numer = num1 - num2 + num3;

            unsigned __int128 denom = (unsigned __int128)(nn - 1) *
                                      (unsigned __int128)(nn + 5);

            cache[i] = (seq_t)(numer / denom);
        }
    }
    max_computed = (size_t)n;
    return cache[n];
}
