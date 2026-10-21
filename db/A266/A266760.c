/*
 * A266760  Growth series for affine Coxeter group (or affine Weyl group) D_5.
 * https://oeis.org/A266760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of t^n in (1+t)(1+t^3)(1+t+⋯+t^7) /
 *            ((1-t^7)(1-t)^4). Recurrence (for n≥12): a(n) = 4a(n‑1) –
 *            6a(n‑2) + 4a(n‑3) – a(n‑4) + a(n‑7) – 4a(n‑8) + 6a(n‑9) –
 *            4a(n‑10) + a(n‑11)
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 387, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    1,    6,   20,   51,  111, 216, 386, 646, 1026, 1560, 2287, 3251
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cache_size = 0;     /* number of terms actually computed */
    static size_t   capacity   = 0;     /* allocated bytes / sizeof(seq_t) */

    /* initialise cache with the first 12 terms */
    if (cache_size == 0) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_size = 12;
    }

    /* return immediately if requested term is already known */
    if ((idx_t)cache_size > n)
        return cache[(size_t)n];

    /* grow the table if needed */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        capacity = needed * 2;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* fill in missing terms */
    for (size_t i = cache_size; i < needed; ++i) {
        if (i < 12) {
            cache[i] = init_vals[i];
            continue;
        }

        long long pos = 0, neg = 0;
        /* positive contributions */
        pos += 4LL * (long long)cache[i - 1];
        pos += 4LL * (long long)cache[i - 3];
        pos += (long long)cache[i - 7];
        pos += 6LL * (long long)cache[i - 9];
        pos += (long long)cache[i - 11];
        /* negative contributions */
        neg += 6LL * (long long)cache[i - 2];
        neg += (long long)cache[i - 4];
        neg += 4LL * (long long)cache[i - 8];
        neg += 4LL * (long long)cache[i - 10];

        long long val = pos - neg;   /* guaranteed non‑negative */
        cache[i] = (seq_t)val;
    }
    cache_size = needed;
    return cache[(size_t)n];
}
