/*
 * A297713  Number of minimal edge covers in the n-dipyramidal graph.
 * https://oeis.org/A297713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) -12*a(n-3) -15*a(n-4) +24*a(n-5)
 *            +49*a(n-6) +6*a(n-7) -73*a(n-8) -76*a(n-9) +5*a(n-10)
 *            +80*a(n-11)+72*a(n-12)+14*a(n-13)
 *            -30*a(n-14)-34*a(n-15)-19*a(n-16)-6*a(n-17)-a(n-18) for n>18.
 *            Initial values: a(1)=1, a(2)=6, a(3)=24, a(4)=74, a(5)=180,
 *            a(6)=464, a(7)=1113, a(8)=264...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices < 1 (the sequence has no defined value there). */
    if (n == 0) return (seq_t)0;

    /* Static cache containing already computed terms (index 0 unused). */
    static seq_t *cache = NULL;
    static idx_t  cache_size   = 0;   /* allocated capacity (≥ n+1 when needed) */
    static idx_t  computed_len = 0;   /* largest n for which cache[n] is known */

    /* One‑time initialization: store first 18 known terms. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 19);   /* indices 0..18 */
        if (!cache) exit(EXIT_FAILURE);
        static const seq_t init[18] = {
            (seq_t)1,  (seq_t)6,   (seq_t)24,  (seq_t)74,   (seq_t)180,
            (seq_t)464, (seq_t)1113, (seq_t)2646,  (seq_t)6360, (seq_t)15222,
            (seq_t)36795, (seq_t)89584, (seq_t)219635, (seq_t)542320,
            (seq_t)1346881, (seq_t)3361998, (seq_t)8427172, (seq_t)21195416
        };
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 18; ++i) cache[i] = init[i-1];
        cache_size   = 19;
        computed_len = 18;
    }

    /* Grow cache if we have to store a higher index. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Cached value already known. */
    if (n <= computed_len) return cache[n];

    /* Compute the sequence up to n, using the recurrence. */
    for (idx_t i = computed_len + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* Positive contributions. */
        pos += (seq_t)4  * cache[i-1];
        pos += (seq_t)1  * cache[i-2];
        pos += (seq_t)24 * cache[i-5];
        pos += (seq_t)49 * cache[i-6];
        pos += (seq_t)6  * cache[i-7];
        pos += (seq_t)5  * cache[i-10];
        pos += (seq_t)80 * cache[i-11];
        pos += (seq_t)72 * cache[i-12];
        pos += (seq_t)14 * cache[i-13];

        /* Negative contributions. */
        neg += (seq_t)12 * cache[i-3];
        neg += (seq_t)15 * cache[i-4];
        neg += (seq_t)73 * cache[i-8];
        neg += (seq_t)76 * cache[i-9];
        neg += (seq_t)30 * cache[i-14];
        neg += (seq_t)34 * cache[i-15];
        neg += (seq_t)19 * cache[i-16];
        neg += (seq_t)6  * cache[i-17];
        neg += (seq_t)1  * cache[i-18];

        /* Always positive by the nature of the recurrence. */
        cache[i] = pos - neg;
    }

    computed_len = n;
    return cache[n];
}
