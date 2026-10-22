/*
 * A392352  a(n) = Sum_{k=0..floor(3*n/17)} binomial(3*n-16*k,k).
 * https://oeis.org/A392352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(3*n/17)} binomial(3*n-16*k,k) Recurrence:
 *            a(n) = 2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+2*a(n-5)+a(n-6)
 *            -a(n-7)+a(n-8)-a(n-9)+a(n-10)-a(n-11)+a(n-12)
 *            -a(n-13)+a(n-14)-a(n-15)+a(n-16) Initial values:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=1,a(6)=3,a(7)=6,
 *            a(8)=9,a(9)=12,a(10)=...
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 61, u64 n <= 120,
 *            u128 n <= 238, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"  /* for ensure_capacity helper */

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated size (in elements) */
static idx_t  computed_max = 0; /* highest index computed in cache */

/* Implements OEIS sequence A392352 */
seq_t a(idx_t n) {
    /* Initialize cache with base values when first called */
    if (computed_max == 0) {
        /* need at least 17 entries for the initial terms */
        size_t need = 17;
        if (cache_size < need)
            ensure_capacity(need, &cache_size, (void **)&cache, sizeof(seq_t));

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 1;
        cache[6] = 3;
        cache[7] = 6;
        cache[8] = 9;
        cache[9] = 12;
        cache[10] = 15;
        cache[11] = 18;
        cache[12] = 27;
        cache[13] = 45;
        cache[14] = 72;
        cache[15] = 108;
        cache[16] = 153;
        computed_max = 16;
    }

    /* If we need more space, grow the cache */
    if (n >= cache_size) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_size)
            ensure_capacity(needed, &cache_size, (void **)&cache, sizeof(seq_t));
    }

    /* If n already computed, return it immediately */
    if (n <= computed_max)
        return cache[n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions */
        pos += cache[i-1];
        pos += cache[i-1];
        pos += cache[i-3];
        pos += cache[i-3];
        pos += cache[i-5];
        pos += cache[i-5];
        pos += cache[i-6];
        pos += cache[i-8];
        pos += cache[i-10];
        pos += cache[i-12];
        pos += cache[i-14];
        pos += cache[i-16];

        /* Negative contributions */
        neg += cache[i-2];
        neg += cache[i-2];
        neg += cache[i-4];
        neg += cache[i-4];
        neg += cache[i-7];
        neg += cache[i-9];
        neg += cache[i-11];
        neg += cache[i-13];
        neg += cache[i-15];

        cache[i] = pos - neg;
    }

    computed_max = n;
    return cache[n];
}
