/*
 * A339687  a(n) = Sum_{d|n} 7^(d-1).
 * https://oeis.org/A339687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 7^(d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for powers of 7: pow7[e] = 7^e  (0‑indexed exponent) */
static seq_t *pow7 = NULL;
static idx_t pow7_cap = 0;

static void ensure_pow7_cap(idx_t exp) {
    if (exp + 1 <= pow7_cap)
        return;
    idx_t new_cap = exp + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (pow7) {
        for (idx_t k = 0; k < pow7_cap; ++k)
            new_arr[k] = pow7[k];
    }
    /* initialise new entries */
    for (idx_t k = pow7_cap; k < new_cap; ++k) {
        if (k == 0)
            new_arr[0] = (seq_t)1;
        else
            new_arr[k] = new_arr[k - 1] * (seq_t)7;
    }
    free(pow7);
    pow7 = new_arr;
    pow7_cap = new_cap;
}

/* cache for computed a(n) values (index 0 unused) */
static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;

static void ensure_a_cache(idx_t n) {
    if (n <= a_cache_cap)
        return;
    idx_t new_cap = n;
    seq_t *new_arr = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (a_cache) {
        for (idx_t k = 0; k <= a_cache_cap; ++k)
            new_arr[k] = a_cache[k];
    }
    for (idx_t k = a_cache_cap + 1; k <= new_cap; ++k)
        new_arr[k] = (seq_t)0;
    free(a_cache);
    a_cache = new_arr;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, return 0 safely */

    if (a_cache && n <= a_cache_cap && a_cache[n] != (seq_t)0)
        return a_cache[n];

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            ensure_pow7_cap(d1 - 1);
            sum += pow7[d1 - 1];

            idx_t d2 = n / i;
            if (d2 != d1) {
                ensure_pow7_cap(d2 - 1);
                sum += pow7[d2 - 1];
            }
        }
    }

    ensure_a_cache(n);
    a_cache[n] = sum;
    return sum;
}
