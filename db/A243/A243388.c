/*
 * A243388  Number of length n+3 0..7 arrays with no four elements in a row
 *          with pattern abba (with a!=b) and new values 0..7 introduced in
 *          0..7 order.
 * https://oeis.org/A243388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the 19‑term linear recurrence a(n) = 29*a(n-1)
 *            –349*a(n-2)+2287*a(n-3) –9065*a(n-4)+23201*a(n-5)
 *            –41175*a(n-6)+55373*a(n-7) –60041*a(n-8)+53229*a(n-9)
 *            –39663*a(n-10) +25113*a(n-11) –13424*a(n-12)+6232*a(n-13)
 *            –2386*a(n-14)+806*a(n-15) –209*a(n-16)+49*a(n-17)
 *            –7*a(n-18)+a(n-19) wi...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* 19‑term recurrence coefficients (a(n–i) coefficient for i=1..19) */
static const int coeff[19] = {
    29,  -349,  2287,  -9065,  23201,
   -41175, 55373, -60041, 53229, -39663,
    25113, -13424,   6232, -2386,   806,
    -209,    49,     -7,      1
};

/* Initial values a(1)...a(19) */
static const seq_t init_vals[19] = {
    (seq_t)14,
    (seq_t)46,
    (seq_t)174,
    (seq_t)737,
    (seq_t)3435,
    (seq_t)17402,
    (seq_t)94907,
    (seq_t)552660,
    (seq_t)3409915,
    (seq_t)22122613,
    (seq_t)149763921,
    (seq_t)1050072965,
    (seq_t)7573195764,
    (seq_t)55841366002,
    (seq_t)418835322674,
    (seq_t)3182422956259,
    (seq_t)24417445081110,
    (seq_t)188710025928915,
    (seq_t)1466319454136469
};

/* One‑based cache: cache[1] = a(1) */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of allocated slots (>= cache[0] unused) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset, but safe */

    /* Ensure the cache is at least 20 slots (0..19) and initialized with the
       first 19 terms. */
    if (cache == NULL) {
        cache_size = 20;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        for (idx_t i = 1; i <= 19; ++i)
            cache[i] = init_vals[i - 1];
    }

    /* Extend the cache if the requested index exceeds the current size. */
    if ((idx_t)cache_size <= n) {
        size_t new_size = n + 1;          /* needed slots: 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        /* If we allocated more space than previous, initialize the new part
           to 0; we will compute only the terms that are newly required. */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t k = 20; k <= n; ++k) {   /* 19 initial terms are ready */
        seq_t val = 0;
        /* Apply the recurrence using unsigned arithmetic for multiplicands. */
        for (int i = 0; i < 19; ++i) {
            seq_t mult = cache[k - 1 - i];
            /* The coefficient is always non‑negative here */
            int c = coeff[i] >= 0 ? coeff[i] : -coeff[i];
            seq_t prod = mult * (seq_t)c;
            if (coeff[i] >= 0)
                val += prod;
            else
                val -= prod;
        }
        cache[k] = val;
    }
    return cache[n];
}
