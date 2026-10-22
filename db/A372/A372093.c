/*
 * A372093  Number of Eulerian orientations of the torus grid graph C_5 X C_n.
 * https://oeis.org/A372093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 219*a(n-2) + 988*a(n-3) - 2407*a(n-4) +
 *            3181*a(n-5) * - 2042*a(n-6) + 292*a(n-7) + 280*a(n-8) -
 *            96*a(n-9) * Initial values (for n = 1 .. 9): * 64, 308, 2116,
 *            16892, 143224, 1250228, 11091536, * 99371772, 895878604
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* holds a(1)...a(k) in cache[0]..cache[k-1] */
static size_t cache_size = 0; /* number of terms currently stored */

/* static array of the first 9 terms */
static const seq_t base_terms[9] = {
    (seq_t)64, (seq_t)308, (seq_t)2116, (seq_t)16892, (seq_t)143224,
    (seq_t)1250228, (seq_t)11091536, (seq_t)99371772, (seq_t)895878604
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; undefined for n<1 */

    /* initialise cache on first use */
    if (cache_size == 0) {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort(); /* memory exhaustion is fatal */
        for (size_t i = 0; i < 9; ++i) cache[i] = base_terms[i];
    }

    /* extend cache up to requested index */
    while (cache_size < (size_t)n) {
        size_t idx = cache_size;      /* idx is zero‑based index of a(n0) where n0 = idx+1 */

        /* compute a(n0) using the recurrence */
        seq_t val = 0;
        val += cache[idx - 1] * ((seq_t)24);
        val -= cache[idx - 2] * ((seq_t)219);
        val += cache[idx - 3] * ((seq_t)988);
        val -= cache[idx - 4] * ((seq_t)2407);
        val += cache[idx - 5] * ((seq_t)3181);
        val -= cache[idx - 6] * ((seq_t)2042);
        val += cache[idx - 7] * ((seq_t)292);
        val += cache[idx - 8] * ((seq_t)280);
        val -= cache[idx - 9] * ((seq_t)96);

        /* enlarge cache if necessary */
        if (cache_size == 0) { /* should not happen after init */
            cache_size = 1;
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) abort();
        }
        seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[(size_t)n - 1];
}
