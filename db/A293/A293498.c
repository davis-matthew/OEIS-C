/*
 * A293498  Number of unlabeled semiorders on n points and having dimension at
 *          most 2.
 * https://oeis.org/A293498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence derived from the generating
 *            function: a(n) = 12*a(n-1) - 61*a(n-2) + 172*a(n-3) - 294*a(n-4)
 *            + 311*a(n-5) - 197*a(n-6) + 66*a(n-7) - 7*a(n-8) for n >= 9,
 *            with initial values a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=42,
 *            a(6)=132, a(7)=426, a(8)=1390
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[9] = {0, 1, 2, 5, 14, 42, 132, 426, 1390};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t max_computed = 0;

    if (n == 0) return 0;           /* offset starts at 1 */

    /* Ensure capacity for index n */
    size_t req = (size_t)n + 1;
    if (req > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < req) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first eight terms once */
    if (max_computed == 0) {
        for (size_t i = 1; i <= 8; ++i) cache[i] = init_vals[i];
        cache[0] = 0;
        max_computed = 8;
    }

    /* Return already computed value */
    if (n <= max_computed) return cache[(size_t)n];

    /* Compute remaining terms using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        seq_t pos = ((seq_t)12) * cache[i - 1];
        pos += ((seq_t)172) * cache[i - 3];
        pos += ((seq_t)311) * cache[i - 5];
        pos += ((seq_t)66) * cache[i - 7];

        seq_t neg = ((seq_t)61) * cache[i - 2];
        neg += ((seq_t)294) * cache[i - 4];
        neg += ((seq_t)197) * cache[i - 6];
        neg += ((seq_t)7) * cache[i - 8];

        cache[i] = pos - neg;
    }

    max_computed = n;
    return cache[(size_t)n];
}
