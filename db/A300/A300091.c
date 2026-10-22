/*
 * A300091  Number of n X 3 0..1 arrays with every element equal to 0, 2, 3,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 3 0/1 arrays satisfying the king‑move
 *            adjacency rule with upper‑left element fixed to 0. Empirical
 *            recurrence for n > 20: a(n) = 5 a(n-1) – 4 a(n-2) – 8 a(n-3) – 5
 *            a(n-4) + 35 a(n-5) – 4 a(n-6) – 26 a(n-7) – 10 a(n-8) – 2 a(n-9)
 *            – 8 a(n-10) + 88 a(n-11) + 78 a(n-12) – 176 a(n...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values a(1)...a(20) are hard‑coded. */

#include <stddef.h>                     // for size_t
#include "../utils/grow_seq.h"          // gives void grow_seq(seq_t **, size_t *, size_t)

static seq_t *cache = NULL;            // a[0] unused, a[1] = a(1)
static size_t cap = 0;                 // allocated capacity
static idx_t stored = 0;               // highest n computed in cache

static void init_cache(void)
{
    /* Ensure cache at least holds up to n=20 */
    grow_seq(&cache, &cap, 21);
    /* Set initial values (index 1..20). */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)7;
    cache[4] = (seq_t)19;
    cache[5] = (seq_t)35;
    cache[6] = (seq_t)95;
    cache[7] = (seq_t)225;
    cache[8] = (seq_t)575;
    cache[9] = (seq_t)1563;
    cache[10] = (seq_t)4179;
    cache[11] = (seq_t)11555;
    cache[12] = (seq_t)32259;
    cache[13] = (seq_t)90327;
    cache[14] = (seq_t)255307;
    cache[15] = (seq_t)723255;
    cache[16] = (seq_t)2054259;
    cache[17] = (seq_t)5847519;
    cache[18] = (seq_t)16660707;
    cache[19] = (seq_t)47512875;
    cache[20] = (seq_t)135574255;
    stored = 20;
}

seq_t a(idx_t n)
{
    /* First time – initialise the cache. */
    if (cache == NULL)
        init_cache();

    /* If requested n <= stored, we already have it. */
    if (n <= (idx_t)stored)
        return cache[n];

    /* Extend the cache to hold the new terms. */
    if ((size_t)n + 1 > cap)
        grow_seq(&cache, &cap, (size_t)n + 1);

    /* Compute missing terms using the empirical recurrence. */
    for (idx_t k = stored + 1; k <= n; ++k)
    {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        /* Positive coefficients */
        pos += (seq_t)5 * cache[k - 1];
        pos += (seq_t)35 * cache[k - 5];
        pos += (seq_t)88 * cache[k - 11];
        pos += (seq_t)78 * cache[k - 12];
        pos += (seq_t)40 * cache[k - 15];
        pos += (seq_t)24 * cache[k - 16];
        pos += (seq_t)52 * cache[k - 17];
        /* Negative coefficients */
        neg += (seq_t)4 * cache[k - 2];
        neg += (seq_t)8 * cache[k - 3];
        neg += (seq_t)5 * cache[k - 4];
        neg += (seq_t)4 * cache[k - 6];
        neg += (seq_t)26 * cache[k - 7];
        neg += (seq_t)10 * cache[k - 8];
        neg += (seq_t)2 * cache[k - 9];
        neg += (seq_t)8 * cache[k - 10];
        neg += (seq_t)176 * cache[k - 13];
        neg += (seq_t)62 * cache[k - 14];
        neg += (seq_t)8 * cache[k - 18];
        neg += (seq_t)8 * cache[k - 19];
        /* Final term (positive minus negative). */
        cache[k] = pos - neg;
    }
    stored = n;
    return cache[n];
}
