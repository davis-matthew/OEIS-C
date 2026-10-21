/*
 * A264060  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,-2.
 * https://oeis.org/A264060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for A264060 a(n) = 4*a(n-1) – 4*a(n-2) +
 *            20*a(n-3) + 20*a(n-4) – 60*a(n-5) – 60*a(n-7) – 20*a(n-8) +
 *            20*a(n-9) + 4*a(n-10) + 4*a(n-11) + a(n-12) Initial values (n =
 *            1..12): 1, 8, 64, 216, 729, 3375, 15625, 64000, 262144, 1124864,
 *            4826809, 20346417 The recurrence holds for all...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage: required by the build.  Do NOT mark as static. */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache    = NULL;  /* array holding a(1)..a(max_n) */
    static idx_t  max_n    = 0;     /* highest index computed so far */

    /* Allocate and initialise cache on first call. */
    if (cache == NULL) {
        cache = (seq_t *)calloc(12, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */

        /* Initial terms.  Index 0 in cache corresponds to a(1). */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)8;
        cache[2]  = (seq_t)64;
        cache[3]  = (seq_t)216;
        cache[4]  = (seq_t)729;
        cache[5]  = (seq_t)3375;
        cache[6]  = (seq_t)15625;
        cache[7]  = (seq_t)64000;
        cache[8]  = (seq_t)262144;
        cache[9]  = (seq_t)1124864;
        cache[10] = (seq_t)4826809;
        cache[11] = (seq_t)20346417;
        max_n = 12;
    }

    /* Padding: return 0 for n < 1 (won''t be used by the tests). */
    if (n < 1) return (seq_t)0;

    /* If the requested value is already computed, return it. */
    if (n <= max_n) return cache[n-1];

    /* Extend the cache to size n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;  /* allocation failure */
    cache = new_cache;

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        idx_t idx = i - 1;     /* cache is 0‑based */

        seq_t val = 0;
        val +=   4 * cache[idx-1];
        val -=   4 * cache[idx-2];
        val +=  20 * cache[idx-3];
        val +=  20 * cache[idx-4];
        val -=  60 * cache[idx-5];
        /* a(n-6) has coefficient 0 – skipped */
        val -=  60 * cache[idx-7];
        val -=  20 * cache[idx-8];
        val +=  20 * cache[idx-9];
        val +=   4 * cache[idx-10];
        val +=   4 * cache[idx-11];
        val +=   1 * cache[idx-12];

        cache[idx] = val;
    }

    max_n = n;
    return cache[n-1];
}
