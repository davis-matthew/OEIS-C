/*
 * A391875  a(n) = Sum_{k=0..n} 2^k * binomial(k+2,2) * binomial(2*k,2*(n-k)).
 * https://oeis.org/A391875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(k+2,2) *
 *            binomial(2*k,2*(n-k)) Recurrence: a(n) = 12a(n‑1) – 48a(n‑2) +
 *            88a(n‑3) – 204a(n‑4) + 480a(n‑5) – 384a(n‑6) + 960a(n‑7) –
 *            816a(n‑8) + 704a(n‑9) – 768a(n‑10) + 384a(n‑11) – 64a(n‑12)
 *            Initial values: a(0) … a(11) computed directly by the definition
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"      /* binomial(idx_t n, idx_t k) -> seq_t */

seq_t a(idx_t n)
{
    /* The first 12 terms are computed directly */
    static seq_t init[12];
    static int init_done = 0;
    if (!init_done) {
        for (idx_t i = 0; i < 12; ++i) {
            seq_t s = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t term = ((seq_t)1 << k);               /* 2^k      */
                /* binomial(k+2, 2) = (k+2)(k+1)/2  */
                term *= ((seq_t)(k + 2) * (k + 1) / (seq_t)2);
                /* binomial(2*k, 2*(i-k)) */
                term *= binomial(2 * k, 2 * (i - k));
                s += term;
            }
            init[(size_t)i] = s;
        }
        init_done = 1;
    }

    if (n < 12)
        return init[(size_t)n];

    /* Cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Ensure capacity for at least n+1 terms */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) {
            free(cache);
            return (seq_t)0; /* allocation failure */
        }
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* Initialize cache with the first 12 terms if not yet done */
    if (cache_size == 0) {
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_size = 12;
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t m = cache_size; m <= (size_t)n; ++m) {
        seq_t v0  = 12 * cache[m - 1];
        seq_t v1  = 48 * cache[m - 2];
        seq_t v2  = 88 * cache[m - 3];
        seq_t v3  = 204 * cache[m - 4];
        seq_t v4  = 480 * cache[m - 5];
        seq_t v5  = 384 * cache[m - 6];
        seq_t v6  = 960 * cache[m - 7];
        seq_t v7  = 816 * cache[m - 8];
        seq_t v8  = 704 * cache[m - 9];
        seq_t v9  = 768 * cache[m -10];
        seq_t v10 = 384 * cache[m -11];
        seq_t v11 = 64  * cache[m -12];

        /* Compute positive and negative parts separately to avoid unsigned underflow */
        seq_t pos = v0 + v2 + v4 + v6 + v8 + v10;
        seq_t neg = v1 + v3 + v5 + v7 + v9 + v11;
        cache[m] = pos - neg;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
